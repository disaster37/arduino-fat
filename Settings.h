#ifndef Settings_h
#define Settings_h

// Pin for LCD
static const uint8_t PIN_LCD_RS = 31;
static const uint8_t PIN_LCD_ENABLE = 33;
static const uint8_t PIN_LCD_D0 = 22;
static const uint8_t PIN_LCD_D1 = 23;
static const uint8_t PIN_LCD_D2 = 24;
static const uint8_t PIN_LCD_D3 = 25;
static const uint8_t PIN_LCD_D4 = 26;
static const uint8_t PIN_LCD_D5 = 27;
static const uint8_t PIN_LCD_D6 = 28;
static const uint8_t PIN_LCD_D7 = 29;
static const uint8_t PIN_LCD_CONTRAST = 32;
static const uint8_t PIN_LCD_LED = 30;

// Pin for Button start
static const uint8_t PIN_BUTTON_START = 35;
static const uint8_t PIN_BUTTON_START_LCD = 34;

// Pin for Button stop
static const uint8_t PIN_BUTTON_STOP = 37;
static const uint8_t PIN_BUTTON_STOP_LCD = 36;

// Pin for Button force barrel motor
static const uint8_t PIN_BUTTON_BARREL = 43;
static const uint8_t PIN_BUTTON_BARREL_LCD = 42;

// Pin for Button force pump
static const uint8_t PIN_BUTTON_PUMP = 45;
static const uint8_t PIN_BUTTON_PUMP_LCD = 44;

// Pin for Button force washing cycle
static const uint8_t PIN_BUTTON_CYCLE = 39;
static const uint8_t PIN_BUTTON_CYCLE_LCD = 38;

// Pin for Button down LCD display
static const uint8_t PIN_BUTTON_LCD_DISPLAY_DOWN = 41;
static const uint8_t PIN_BUTTON_LCD_DISPLAY_DOWN_LCD = 40;

// Pin for watter captor
static const uint8_t PIN_WATTER_CAPTOR_TOP = 48;
static const uint8_t PIN_WATTER_CAPTOR_DOWN = 49;

// Pin for watter security captor
static const uint8_t PIN_WATTER_SECURITY_CAPTOR_TOP = 50;
static const uint8_t PIN_WATTER_SECURITY_CAPTOR_DOWN = 51;

// Pin for tempeture / humidity
static const uint8_t PIN_DHT = 5;

// Pin for motor barrel
static const uint8_t PIN_MOTOR_BARREL = 46; 

// Pin for motor pump
static const uint8_t PIN_MOTOR_PUMP = 47;

#endif
