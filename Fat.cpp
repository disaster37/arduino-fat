#include "Arduino.h"
#include "Fat.h"
#include "Bounce2.h"
#include "PinOutput.h"
#include "LiquidCrystal.h"
#include "DHTesp.h"



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
  _washingDuration= 30000;
  _washingStartTime = 0;
  _isWashing = false;
  _isForceMotorPump = false;
  _isForceMotorBarrel = false;
  _isModeAuto = true;
  _isSecurity = false;
  _isLedLight = true;
  _messagePosition = 0;
  _displayDuration = millis();
  _ledDuration = 60000;
  _ledStartTime = millis();
  _readCaptorDH11StartTime = millis();
  _lastWhasingTime = 0;

  _currentPositionInListLastWashingTime = 0;
  for(int i = 0; i < SIZE_AVERAGE_WASHING_TIME; i++) {
    _listLastWashingTime[i] = 0;
  }

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

  unsigned long startTime;

  if (_washingStartTime == 0) {
    _washingStartTime = millis();
  }
  
  startTime = millis();
  // Washing is finished
  if((startTime - _washingStartTime) > _washingDuration) {
    _stopMotorPump();
    _stopMotorBarrel();
    _washingStartTime = 0;
    _addWashingDuration(_lastWhasingTime);
    _lastWhasingTime = millis();
    _isWashing = false;
  }
  
  // Washing is running
  else {
    _startMotorPump();
    _startMotorBarrel();
    _isWashing = true;
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

  unsigned long currentTime = millis();

  

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
    state = "Wash " + String(_currentDelayWashing());
  }

  if (!_isSecurity && !_isWashing && !_isForceMotorPump && !_isForceMotorBarrel) {
    state = "Off";
  }

  
  _message[0] = String("Mode: ") + mode;
  _message[1] = String("State: ") + state;
  _message[2] = String("Washing: ") + String(_lastWashingTimeInMinutes()) + String("min");
  _message[3] = String("Avg: ") + String(_getAverageWashingDurationInMinutes()) + String("min");

  // Check to update value
  if((currentTime - _readCaptorDH11StartTime) > CAPTOR_TEMP_HUMIDITY_REFRESH_MILLIS) {
    _message[4] = String("Tempeture: ") + (int)_captorTempetureHumidity.getTemperature();
    _message[5] = String("Humidity: ") + (int)_captorTempetureHumidity.getHumidity();
    _readCaptorDH11StartTime = millis();
  }
  _message[6] = String("Captor1 T: ") + (!_captorWatterTop.read());
  _message[7] = String("Cpator1 D: ") + (_captorWatterDown.read());
  _message[8] = String("Captor2 T: ") + (!_captorWatterSecurityTop.read());
  _message[9] = String("Captor2 D: ") + (_captorWatterSecurityDown.read());
  
  // Display message
  if((currentTime - _displayDuration) > LCD_REFRESH_MILLIS) {
    _displayDuration = millis();
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
  _washingDuration = duration * 1000;
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
}

/**
 * Permit to manage all led (button and lcd)S
 */
void Fat::_manageLed() {

  // Comptute the led state
  if (_buttonStart.fell() || _buttonStop.fell() || _buttonForceMotorBarrel.fell() || _buttonForceMotorPump.fell() || _buttonForceWash.fell() || _buttonLCDDisplayDown.fell()) {
    _ledStartTime = millis();
    _isLedLight = true;
  }

  if (_isLedLight && ((millis() - _ledStartTime) > _ledDuration)) {
    _isLedLight = false;
  }

  Serial.println(_ledDuration);


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

/**
 * Permit to get the duration between now and the last washing in minutes
 */
long Fat::_lastWashingTimeInMinutes(){

  return ((millis() - _lastWhasingTime) / 60000);
  
}

void Fat::_addWashingDuration(unsigned long lastWashingTime) {

  if (lastWashingTime != 0) {

    if (_currentPositionInListLastWashingTime + 1 == SIZE_AVERAGE_WASHING_TIME) {
      _currentPositionInListLastWashingTime = 0;
    }
    else {
      _currentPositionInListLastWashingTime++;
    }
  
    _listLastWashingTime[_currentPositionInListLastWashingTime] = (millis() - lastWashingTime) / 60000;

  }
}

long Fat::_getAverageWashingDurationInMinutes() {

  int average = 0;
  int nbItem = 0;
  
  for(int i = 0; i < SIZE_AVERAGE_WASHING_TIME; i++) {
    if(_listLastWashingTime[i] != 0) {
      average += _listLastWashingTime[i];
      nbItem++;
    }
    else {
      break;
    }
  }

  return (average/nbItem);
}

/**
 * Return the number of second that stay before the washing ended
 */
int Fat::_currentDelayWashing(){


  unsigned long currentTime;

  if (_washingStartTime == 0) {
    return 0;
  }
  
  currentTime = millis();

  return int((_washingDuration - (currentTime - _washingStartTime))/1000);
  
  
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
  Serial.println(String("Last washing time: ") + _lastWashingTimeInMinutes() + String(" min"));
  
  delay(2000);
}


