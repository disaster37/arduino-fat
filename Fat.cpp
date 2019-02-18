#include "Arduino.h"
#include "Fat.h"
#include "Bounce2.h"
#include "PinOutput.h"
#include "LiquidCrystal.h"
#include "DHTesp.h"
#include "timer.h"
#include "duration.h"



/**
 * Constructor
 */
Fat::Fat()
{

  // Button
  _buttonStart = Bounce();
  _buttonStop = Bounce();
  _buttonForceMotorBarrel = Bounce();
  _buttonForceMotorPump = Bounce();
  _buttonForceWash = Bounce();
  _buttonLCDDisplayDown = Bounce();

  // Watter Captor
  _captorWatterTop = Bounce();
  _captorWatterDown = Bounce();
  _captorWatterSecurityTop = Bounce();
  _captorWatterSecurityDown = Bounce();

  // Humidity & tempture captor
  _captorTempetureHumidity = DHTesp();
  
  // output
  _motorBarrel = PinOutput();
  _motorPump = PinOutput();
  _buttonStartLed = PinOutput();
  _buttonStopLed = PinOutput();
  _buttonForceMotorBarrelLed = PinOutput();
  _buttonForceMotorPumpLed = PinOutput();
  _buttonForceWashLed = PinOutput();
  _buttonLCDDisplayDownLed = PinOutput();

  // Variable init
  _isWashing = false;
  _isForceMotorPump = false;
  _isForceMotorBarrel = false;
  _isModeAuto = true;
  _isSecurity = false;
  _isLedLight = true;
  _messagePosition = 0;

  // Timers
  _timerWash = Timer();
  _timerWash.setTimerInSecond(30);
  _timerLCDRefresh = Timer();
  _timerLCDRefresh.setTimerInMilli(LCD_REFRESH_MILLIS);
  _timerCaptorHumidityRefresh();
  _timerCaptorHumidityRefresh.SetTimerInMilli(CAPTOR_TEMP_HUMIDITY_REFRESH_MILLIS);
  _timerLedLightDuration = Timer();
  _timerLedLightDuration.SetTimerInSecond(60);
  
  // Durations
  _durationWash = Duration();
  _durationWash.setHistorySize(10);

}

/**
 * Attach the motor barrel pin for output
 */
void Fat::attachMotorBarrel(uint8_t pin){
  _motorBarrel.attach(pin);
}

/**
 * Attach the motor pump pin for output
 */
void Fat::attachMotorPump(uint8_t pin){
  _motorPump.attach(pin);
}

/**
 * Attach the start button pin
 */
void Fat::attachButtonStart(uint8_t pin, uint8_t pinLed) {
  _buttonStart.attach(pin, INPUT_PULLUP);
  _buttonStartLed.attach(pinLed);
}

/**
 * Attach the stop button pin
 */
void Fat::attachButtonStop(uint8_t pin, uint8_t pinLed) {
  _buttonStop.attach(pin, INPUT_PULLUP);
  _buttonStopLed.attach(pinLed);
}

/**
 * Attach the pin of the force start motor barrel button 
 */
void Fat::attachButtonForceMotorBarrel(uint8_t pin, uint8_t pinLed) {
  _buttonForceMotorBarrel.attach(pin, INPUT_PULLUP);
  _buttonForceMotorBarrelLed.attach(pinLed);
 }

/**
 * Attach the pin of the force start motor pump button
 */
void Fat::attachButtonForceMotorPump(uint8_t pin, uint8_t pinLed) {
  _buttonForceMotorPump.attach(pin, INPUT_PULLUP);
  _buttonForceMotorPumpLed.attach(pinLed);
 }

/**
 * Attach the pin of force washing button
 * The washing during one cycle
 */
void Fat::attachButtonForceWashing(uint8_t pin, uint8_t pinLed) {
  _buttonForceWash.attach(pin, INPUT_PULLUP);
  _buttonForceWashLed.attach(pinLed);
}

/**
 * Attach the pin to down message on LCD display
 */
void Fat::attachButtonLCDDisplayDown(uint8_t pin, uint8_t pinLed) {
  _buttonLCDDisplayDown.attach(pin, INPUT_PULLUP);
  _buttonLCDDisplayDownLed.attach(pinLed);
}

/**
 * Attach the 2 pin of the normal watter captor (the higher)
 */
void Fat::attachCaptorWatter(uint8_t pinCaptorTop, uint8_t pinCaptorDown) {
  _captorWatterTop.attach(pinCaptorTop, INPUT_PULLUP);
  _captorWatterDown.attach(pinCaptorDown, INPUT_PULLUP);
}

/**
 * Attach the 2 pin of the security watter captor (the lower)
 */
void Fat::attachCaptorWatterSecurity(uint8_t pinCaptorTop, uint8_t pinCaptorDown) {
  _captorWatterSecurityTop.attach(pinCaptorTop, INPUT_PULLUP);
  _captorWatterSecurityDown.attach(pinCaptorDown, INPUT_PULLUP);
}

/**
 * Attach the LCD pin to display on LCD
 */
void Fat::attachLCD(uint8_t pinRS, uint8_t pinEnable, uint8_t pinD0, uint8_t pinD1, uint8_t pinD2, uint8_t pinD3, uint8_t pinD4, uint8_t pinD5, uint8_t pinD6, uint8_t pinD7, uint8_t pinContrast, uint8_t pinLed) {
  _lcd = LiquidCrystal(pinRS, pinEnable, pinD0, pinD1, pinD2, pinD3, pinD4, pinD5, pinD6, pinD7);
  _lcdLed.attach(pinLed);

  analogWrite(pinContrast, 15);
  _lcd.begin(16, 2); // règle la taille du LCD : 16 colonnes et 2 lignes
  _lcd.display();
}

/**
 * Attach the DHT11 captor
 */
void Fat::attachTempetureHumidityCaptor(uint8_t pin) {
  _captorTempetureHumidity.setup(pin, DHTesp::DHT11);
}

/**
 * Start the barrel motor
 */
void Fat::_startMotorBarrel() {
  _motorBarrel.toUp();
}

/**
 * Stop the barrel motor
 */
void Fat::_stopMotorBarrel() {
  _motorBarrel.toDown();
}

/**
 * Start the pump motor
 */
void Fat::_startMotorPump() {
  _motorPump.toUp();
}

/**
 * Stop the pump motor
 */
void Fat::_stopMotorPump() {
  _motorPump.toDown();
}

/**
 * Start the washing
 * The waching continue during _wachingDurationInSecond (30 seconds per default)
 * When it wash, the pump motor and the pump barrel run in the same time
 */
void Fat::_wash() {

  // Washing is finished
  if(_timerWash.isJustFinished()) {
    _stopMotorPump();
    _stopMotorBarrel();
    _isWashing = false;
    _durationWash.start();
  } 
  // Washing is running
  else if(_timerWash.isRun()) {
    _startMotorPump();
    _startMotorBarrel();
    _isWashing = true;
  } else {
  // Start new washing cycle
    _timerWash.start();
    _durationWash.stop()
  }
}

void Fat::displayMessage() {

  // Manage position
  if(_buttonLCDDisplayDown.fell()) {
    if(_messagePosition < (SIZE_MESSAGE -1)) {
      _messagePosition++;
    }
    else {
      _messagePosition = 0;
    }
  }
  
  // Compose all messages
  String mode = "";
  String state = "";

  if (_isModeAuto) {
    mode = "Auto";
  }

  if (_isForceMotorPump) {
    mode = "F Pump";
    state = "Force";
  }

  if (_isForceMotorBarrel) {
    mode = "F Barrel";
    state = "Force";
  }

  if (!_isModeAuto && !_isForceMotorPump && !_isForceMotorBarrel) {
    mode = "Stop";
  }

  if (_isSecurity) {
    state = "Security";
  }

  if (_isWashing) {
    state = "Wash " + String(_timerWash.getCurrentValueInSecond());
  }

  if (!_isSecurity && !_isWashing && !_isForceMotorPump && !_isForceMotorBarrel) {
    state = "Off";
  }

  
  _message[0] = String("Mode: ") + mode;
  _message[1] = String("State: ") + state;
  _message[2] = String("Washing: ") + String(_durationWash.getCurrentValueInMinute()) + String("min");
  _message[3] = String("Last: ") + String(_durationWash.getLastValueFromHistoryInMinute()) + String("min");
  _message[4] = String("Avg: ") + String(_durationWash.getAvgFromHistoryInMinute()) + String("min");

  // Check to update humidity captor value
  if(_timerCaptorHumidityRefresh.isJustFinished()) {
    _message[5] = String("Tempeture: ") + (int)_captorTempetureHumidity.getTemperature();
    _message[6] = String("Humidity: ") + (int)_captorTempetureHumidity.getHumidity();
  } 
  else if(_timerCaptorHumidityRefresh.isRun() == false) {
    _timerCaptorHumidityRefresh.start();
  }
  
  _message[7] = String("Captor1 T: ") + (!_captorWatterTop.read());
  _message[8] = String("Cpator1 D: ") + (_captorWatterDown.read());
  _message[9] = String("Captor2 T: ") + (!_captorWatterSecurityTop.read());
  _message[10] = String("Captor2 D: ") + (_captorWatterSecurityDown.read());
  
  // Display message
  if(_timerLCDRefresh.isJustFinished()) {
    _lcd.clear();
    char temp[16];
    _lcd.setCursor(0,0);
    _message[_messagePosition].toCharArray(temp, 16);
    _lcd.write(temp);
  
    _lcd.setCursor(0, 1);
    if(_messagePosition < (SIZE_MESSAGE -1)) {
      _message[_messagePosition + 1].toCharArray(temp, 16);
      _lcd.write(temp);
    }
    else {
       String("               ").toCharArray(temp, 16);
       _lcd.write(temp);
    }
    
  } else if (_timerLCDRefresh.isRun() == false) {
    // Run the timer
    _timerLCDRefresh.start();
  }
}

/**
 * The main function that check if fonction is needed depend to button or captor
 */
void Fat::run() {

  _updateInputState();
  _manageLed();
  displayMessage();

  // Check if stop all is needed
  if (_buttonStop.fell() == true) {
     _isModeAuto = false;
     _stopMotorPump();
     _stopMotorBarrel();
     _isWashing = false;
  }
  

  // Check if force pump is needed
  if (_buttonForceMotorPump.fell()){
    _isForceMotorPump = true;
    _startMotorPump();
    
  }

  // Check if force pump is finished
  if (_buttonForceMotorPump.rose()){
    _stopMotorPump();
    _isForceMotorPump = false;
  }

  // Check if force barrel is needed
  if (_buttonForceMotorBarrel.fell()){
    _isForceMotorBarrel = true;
    _startMotorBarrel();
  }

  // Check if force barrel is finished
  if (_buttonForceMotorBarrel.rose()){
    _isForceMotorBarrel = false;
    _stopMotorBarrel();
  }

  // Check if force washing is needed
  if (_buttonForceWash.fell() && _isModeAuto) {
    _wash();
  }

  // Check if auto mode is needed
  if (_buttonStart.fell()) {
    _isModeAuto = true;
  }


  // Check if auto mode can running
  if (_isModeAuto && !_isForceMotorBarrel && !_isForceMotorPump) {
    
    // Check if washing is already started
    if (_isWashing == true) {
      _wash();
    }

    // Check the captor state
    else {

      // Check the security captor state
      if (_captorWatterSecurityDown.read() == LOW) {
        _stopMotorPump();
        _stopMotorBarrel();
        _isSecurity = true;
      }
      else {
        _isSecurity = false;
        if (_captorWatterTop.rose() || _captorWatterDown.fell() || _captorWatterSecurityTop.rose()) {
          _wash();
        }
      } 
    }
  }

  
  
}

/**
 * Permit to set the duration of washing cycle in second
 * Default to 30s
 */
void Fat::setWashingDurationInSecond(int duration) {
  _timerWash.setTimerInSecond(duration);
}

/**
 * Permit to update the buttons state
 */
void Fat::_updateInputState() {

  _buttonStart.update();
  _buttonStop.update();
  _buttonForceMotorBarrel.update();
  _buttonForceMotorPump.update();
  _buttonForceWash.update();
  _buttonLCDDisplayDown.update();
  _captorWatterTop.update();
  _captorWatterDown.update();
  _captorWatterSecurityTop.update();
  _captorWatterSecurityDown.update();
  
  _timerWash.update();
  _timerLCDRefresh.update();
  _timerCaptorHumidityRefresh.update();
  _timerLedLightDuration.update();
  _durationWash.update();
}

/**
 * Permit to manage all led (button and lcd)S
 */
void Fat::_manageLed() {

  // Comptute the led state
  if (_buttonStart.fell() || _buttonStop.fell() || _buttonForceMotorBarrel.fell() || _buttonForceMotorPump.fell() || _buttonForceWash.fell() || _buttonLCDDisplayDown.fell()) {
    _timerLedLightDuration.start();
    _isLedLight = true;
  }

  if (_timerLedLightDuration.isJustFinished()) {
    _isLedLight = false;
  }

  Serial.println(_timerLedLightDuration.getCurrentValueInSecond);


  if(_isLedLight) {
    _buttonStartLed.toUp();
    _buttonStopLed.toUp();
    _buttonForceMotorBarrelLed.toUp();
    _buttonForceMotorPumpLed.toUp();
    _buttonForceWashLed.toUp();
    _buttonLCDDisplayDownLed.toUp();
    _lcdLed.toUp();
  }
  else{
    _buttonStartLed.toDown();
    _buttonStopLed.toDown();
    _buttonForceMotorBarrelLed.toDown();
    _buttonForceMotorPumpLed.toDown();
    _buttonForceWashLed.toDown();
    _buttonLCDDisplayDownLed.toDown();
    _lcdLed.toDown();
  }
}




void Fat::debug(){

  _updateInputState();

  // Display debug message
  _lcd.setCursor(0,0);
  char debug[16] = "DEBUG";
  _lcd.write(debug);

  // put all leds on
  _isLedLight = true;
  _manageLed();

  // Display button state
  Serial.println(String("Btn Start: ") + (!_buttonStart.read()));
  Serial.println(String("Btn Stop: ") + (!_buttonStop.read()));
  Serial.println(String("Btn Force wash: ") + (!_buttonForceWash.read()));
  Serial.println(String("Btn Force barrel: ") + (!_buttonForceMotorBarrel.read()));
  Serial.println(String("Btn Force pump: ") + (!_buttonForceMotorPump.read()));
  Serial.println(String("Btn menu: ") + (!_buttonLCDDisplayDown.read()));

  // Display captor state
  Serial.println(String("Cpt top: ") + (!_captorWatterTop.read()));
  Serial.println(String("Cpt down: ") + (_captorWatterDown.read()));
  Serial.println(String("Cpt2 top: ") + (!_captorWatterSecurityTop.read()));
  Serial.println(String("Bpt2 down: ") + (_captorWatterSecurityDown.read()));

  // Display relay state
  Serial.println(String("Relay Pump: ") + _motorPump.state());
  Serial.println(String("Relay Barrel: ") + _motorBarrel.state());

  // Display last washing time
  Serial.println(String("Current washing time: ") + _durationWash.getCurrentValueInMinute() + String(" min"));
  Serial.println(String("Last washing time: ") + _durationWash.getLastValueFromHistoryInMinute() + String(" min"));
  Serial.println(String("AVG washing time: ") + _durationWash.getAvgFromHistoryInMinute() + String(" min"));
   
  
  delay(2000);
}


