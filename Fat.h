#ifndef Fat_h
#define Fat_h

#include "Arduino.h"
#include "Bounce2.h"
#include "PinOutput.h"
#include "LiquidCrystal.h"
#include "DHTesp.h"
#include "timer.h"
#include "duration.h"

#define SIZE_MESSAGE 11
#define LCD_REFRESH_MILLIS 200
#define CAPTOR_TEMP_HUMIDITY_REFRESH_MILLIS 2000

class Fat
{
  public:
    Fat();
    void attachMotorBarrel(uint8_t pin);
    void attachMotorPump(uint8_t pin);
    void attachButtonStart(uint8_t pin, uint8_t pinLed);
    void attachButtonStop(uint8_t pin, uint8_t pinLed);
    void attachButtonForceMotorBarrel(uint8_t pin, uint8_t pinLed);
    void attachButtonForceMotorPump(uint8_t pin, uint8_t pinLed);
    void attachButtonForceWashing(uint8_t pin, uint8_t pinLed);
    void attachButtonLCDDisplayDown(uint8_t pin, uint8_t pinLed);
    void attachCaptorWatter(uint8_t pinCaptorTop, uint8_t pinCaptorDown);
    void attachCaptorWatterSecurity(uint8_t pinCaptorTop, uint8_t pinCaptorDown);
    void attachLCD(uint8_t pinRS, uint8_t pinEnable, uint8_t pinD0, uint8_t pinD1, uint8_t pinD2, uint8_t pinD3, uint8_t pinD4, uint8_t pinD5, uint8_t pinD6, uint8_t pinD7, uint8_t pinContrast, uint8_t pinLed);
    void attachTempetureHumidityCaptor(uint8_t pin);
    void run();
    void displayMessage();
    void setWashingDurationInSecond(int duration);
    void debug();
  private:
    PinOutput _motorBarrel;
    PinOutput _motorPump;
    Bounce _buttonStart;
    PinOutput _buttonStartLed;
    Bounce _buttonStop;
    PinOutput _buttonStopLed;
    Bounce _buttonForceMotorBarrel;
    PinOutput _buttonForceMotorBarrelLed;
    Bounce _buttonForceMotorPump;
    PinOutput _buttonForceMotorPumpLed;
    Bounce _buttonForceWash;
    PinOutput _buttonForceWashLed;
    Bounce _buttonLCDDisplayDown;
    PinOutput _buttonLCDDisplayDownLed;
    Bounce _captorWatterTop;
    Bounce _captorWatterDown;
    Bounce _captorWatterSecurityTop;
    Bounce _captorWatterSecurityDown;
    LiquidCrystal _lcd = LiquidCrystal(0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    PinOutput _lcdLed;
    DHTesp _captorTempetureHumidity;
    bool _isWashing;
    bool _isModeAuto;
    bool _isSecurity;
    bool _isForceMotorPump;
    bool _isForceMotorBarrel;
    String _message[SIZE_MESSAGE];
    int _messagePosition;
    bool _isLedLight;
    void _startMotorBarrel();
    void _stopMotorBarrel();
    void _startMotorPump();
    void _stopMotorPump();
    void _wash();
    void _updateInputState();
    void _manageLed();
    Timer _timerWash;
    Timer _timerLCDRefresh;
    Timer _timerCaptorHumidityRefresh;
    Timer _timerLedLightDuration;
    Duration _durationWash;
};

#endif
