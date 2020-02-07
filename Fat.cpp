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
  _lcdContrast = PinOutput();
  _lcdLed = PinOutput();

  // Variable init
  _isWashing = false;
  _isForceMotorPump = false;
  _isForceMotorBarrel = false;
  _isModeAuto = true;
  _isSecurity = false;
  _isLedLight = true;
  _messagePosition = 0;
  _waitTimeForceWashingCycle = 180;
  _waitTimeForceWashingCycleFreeze = 60;
  _humidityValue = 0;
  _tempetureValue = 0;

  // Timers
  _timerWash = Timer();
  _timerWash.setTimerInSecond(30);
  _timerWaitBetweenWash = Timer();
  _timerWaitBetweenWash.setTimerInSecond(60);
  _timerWaitPump = Timer();
  _timerWaitPump.setTimerInSecond(10);
  _timerLCDRefresh = Timer();
  _timerLCDRefresh.setTimerInMilli(LCD_REFRESH_MILLIS);
  _timerCaptorHumidityRefresh = Timer();
  _timerCaptorHumidityRefresh.setTimerInMilli(CAPTOR_TEMP_HUMIDITY_REFRESH_MILLIS);
  _timerLedLightDuration = Timer();
  _timerLedLightDuration.setTimerInSecond(60);
  // Start timer
  _timerCaptorHumidityRefresh.start();
  _timerLCDRefresh.start();
  _timerLedLightDuration.start();
  
  // Durations
  _durationWash = Duration();
  _durationWash.start();

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
  _lcdContrast.attach(pinContrast);

  _lcdLed.toUp();
  _lcdContrast.toUp();
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
    _timerWaitBetweenWash.start();
  }
  // Washing is not yet running
  else if(_timerWash.isFinished()) {
  // Start new washing cycle
    _timerWash.start();
    _timerWaitPump.start();
    _durationWash.stop();
    _startMotorPump();
    _isWashing = true;
  }

  // Need start barrel
  if(_timerWaitPump.isJustFinished()) {
       _startMotorBarrel();
  }
}

void Fat::displayMessage() {

  
  // Manage position
  if(_buttonLCDDisplayDown.fell()) {
    if(_messagePosition < (NUMBER_MESSAGE_LCD -1)) {
      _messagePosition++;
    }
    else {
      _messagePosition = 0;
    }
  }
  
  // Compose all messages
  char mode[30] = "";
  char state[30] = "";
  char end_message[30] = "";

  if (_isModeAuto) {
    if(_timerWaitBetweenWash.isRun()) {
      sprintf(mode, "%s %d", "Auto", _timerWaitBetweenWash.getCurrentValueInSecond());
    }
    else {
      sprintf(mode, "%s", "Auto");
    }
  }

  if (_isForceMotorPump) {
    sprintf(mode, "%s", "F Pump");
    sprintf(state, "%s", "Force");
  }

  if (_isForceMotorBarrel) {
    sprintf(mode, "%s", "F Barrel");
    sprintf(state, "%s", "Force");
  }

  if (!_isModeAuto && !_isForceMotorPump && !_isForceMotorBarrel) {
    sprintf(mode, "%s", "Stop");
  }

  if (_isSecurity) {
    sprintf(state, "%s", "Security");
  }

  if (_isWashing) {
    sprintf(state, "Wash %d", _timerWash.getCurrentValueInSecond());
  }

  if (!_isSecurity && !_isWashing && !_isForceMotorPump && !_isForceMotorBarrel) {
    sprintf(state, "%s", "Off");
  }

  sprintf(_message[0], "Mode: %s", mode);
  sprintf(_message[1], "State: %s", state);
  sprintf(_message[2], "Washing: %d min", _durationWash.getCurrentValueInMinute());
  sprintf(_message[3], "Last: %d min", _durationWash.getLastValueFromHistoryInMinute());
  sprintf(_message[4], "Avg: %d min", _durationWash.getAvgFromHistoryInMinute());
  sprintf(_message[5], "Tempeture: %d", _tempetureValue);
  sprintf(_message[6], "Humidity: %d", _humidityValue);
  sprintf(_message[7], "Captor1 T: %d", !_captorWatterTop.read());
  sprintf(_message[8], "Cpator1 D: %d", _captorWatterDown.read());
  sprintf(_message[9], "Captor2 T: %d", !_captorWatterSecurityTop.read());
  sprintf(_message[10], "Captor2 D: %d", _captorWatterSecurityDown.read());

  // Display message
  if(_timerLCDRefresh.isJustFinished()) {
    _lcd.clear();
    _lcd.print(_message[_messagePosition]);
  
    _lcd.setCursor(0, 1);
    if(_messagePosition < (NUMBER_MESSAGE_LCD -1)) {
      _lcd.print(_message[_messagePosition + 1]);
    }
    else {
       _lcd.print(end_message);
    }
    
    // Run timer again
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

    // Check the captor state only if washing is not already started
    if (_isWashing) {
      _wash();
    }
    else {

      // Check the security captor state
      if (_captorWatterSecurityDown.read() == LOW) {
        _stopMotorPump();
        _stopMotorBarrel();
        _isSecurity = true;
      }
      else {
        _isSecurity = false;
        if ((_captorWatterTop.rose() || _captorWatterDown.fell() || _captorWatterSecurityTop.rose()) && (_timerWaitBetweenWash.isFinished())) {
          _wash();
        }

        // Check if we need to force washing cycle because a long time without clean and the impact of tempeture
        if(_tempetureValue < 0) {
          // Tempeture is under 0 we force small cycle
          if(_durationWash.getCurrentValueInMinute() >= _waitTimeForceWashingCycleFreeze) {
            _wash();
          }
        }
        else {
          // Tempeture is bigger than 0, wee force normal cycle
          if(_durationWash.getCurrentValueInMinute() >= _waitTimeForceWashingCycle) {
            _wash();
          }
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
 * Permit to set duration to wait between washing in second.
 * It's to avoid to wash continuously.
 */
void Fat::setWaitTimeBetweenWasingInSecond(int duration) {
  _timerWaitBetweenWash.setTimerInSecond(duration);
}


/**
 * Permit to set duration to start pump before barrel in second.
 * It's permit to have water before barrel move.
 */
void Fat::setWaitTimePumpInSecond(int duration) {
  _timerWaitPump.setTimerInSecond(duration);
}


/**
 * Permit to set the time to wait before lauch clean cycle.
 */
void Fat::setWaitTimeForceWashingCycleInMinute(int duration) {
  _waitTimeForceWashingCycle = duration;
}

/**
 * Permit to set the time to wait before lauch clean cycle when it frozen.
 */
void Fat::setWaitTimeForceWashingCycleFreezeInMinute(int duration) {
  _waitTimeForceWashingCycleFreeze = duration;
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
  _timerWaitBetweenWash.update();
  _timerWaitPump.update();
  _timerLCDRefresh.update();
  _timerCaptorHumidityRefresh.update();
  _timerLedLightDuration.update();
  _durationWash.update();
  
  // Update tempeture and humidity value
  if(_timerCaptorHumidityRefresh.isJustFinished()) {
    _tempetureValue = (int)_captorTempetureHumidity.getTemperature());
    _humidityValue = (int)_captorTempetureHumidity.getHumidity());
    _timerCaptorHumidityRefresh.start();
  }
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

  if(_isLedLight) {
    _buttonStartLed.toUp();
    _buttonStopLed.toUp();
    _buttonForceMotorBarrelLed.toUp();
    _buttonForceMotorPumpLed.toUp();
    _buttonForceWashLed.toUp();
    _buttonLCDDisplayDownLed.toUp();
    _lcdLed.toUp();
    _lcd.display();
  }
  else{
    _buttonStartLed.toDown();
    _buttonStopLed.toDown();
    _buttonForceMotorBarrelLed.toDown();
    _buttonForceMotorPumpLed.toDown();
    _buttonForceWashLed.toDown();
    _buttonLCDDisplayDownLed.toDown();
    _lcdLed.toDown();
    _lcd.noDisplay();
  }
}

/**
 * Permit to display debug values
 */
void Fat::debug(){

  // Display debug message
  _lcd.setCursor(0,0);
  char debug[16] = "DEBUG";
  _lcd.print(debug);

 
  Serial.println(_message[0]);
  Serial.println(_message[1]);
  Serial.println(_message[2]);
  Serial.println(_message[3]);
  Serial.println(_message[4]);
  Serial.println(_message[5]);
  Serial.println(_message[6]);
  Serial.println(_message[7]);
  Serial.println(_message[8]);
  Serial.println(_message[9]);
  Serial.println(_message[10]);
  Serial.println(_durationWash.getCurrentValueInSecond());
  Serial.println(_durationWash.getLastValueFromHistoryInSecond());
  Serial.println(_durationWash.getAvgFromHistoryInSecond());

}


