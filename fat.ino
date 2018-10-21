

#include "LiquidCrystal.h" //ajout de la librairie
#include "Bounce2.h"
#include "Settings.h"
#include "PinOutput.h"
#include "Ultrasonic.h"
#include "DHTesp.h"
#include <dht11.h>
#include "Fat.h"



// Create FAT object
Fat fat = Fat();
PinOutput ledtest = PinOutput();
Bounce test = Bounce();

void setup()
{
  // Init serial consol for debugging
  Serial.begin(9600);

  // Set the motor pin
  fat.attachMotorBarrel(PIN_MOTOR_BARREL);
  fat.attachMotorPump(PIN_MOTOR_PUMP);

  // Set the button pin
  fat.attachButtonStart(PIN_BUTTON_START, PIN_BUTTON_START_LCD);
  fat.attachButtonStop(PIN_BUTTON_STOP, PIN_BUTTON_STOP_LCD);
  fat.attachButtonForceMotorBarrel(PIN_BUTTON_BARREL, PIN_BUTTON_BARREL_LCD);
  fat.attachButtonForceMotorPump(PIN_BUTTON_PUMP, PIN_BUTTON_PUMP_LCD);
  fat.attachButtonForceWashing(PIN_BUTTON_CYCLE, PIN_BUTTON_CYCLE_LCD);
  fat.attachButtonLCDDisplayDown(PIN_BUTTON_LCD_DISPLAY_DOWN, PIN_BUTTON_LCD_DISPLAY_DOWN_LCD);

  // Set the watter captor pin
  fat.attachCaptorWatter(PIN_WATTER_CAPTOR_TOP, PIN_WATTER_CAPTOR_DOWN);
  fat.attachCaptorWatterSecurity(PIN_WATTER_SECURITY_CAPTOR_TOP, PIN_WATTER_SECURITY_CAPTOR_DOWN);

  // Set the humidity & tempeture pin
  fat.attachTempetureHumidityCaptor(PIN_DHT);

  // Set the LCD pin
  fat.attachLCD(PIN_LCD_RS, PIN_LCD_ENABLE, PIN_LCD_D0, PIN_LCD_D1, PIN_LCD_D2, PIN_LCD_D3, PIN_LCD_D4, PIN_LCD_D5, PIN_LCD_D6, PIN_LCD_D7, PIN_LCD_CONTRAST, PIN_LCD_LED);
  

  // Set the washing duration
  fat.setWashingDurationInSecond(30);

  ledtest.attach(34);
  test.attach(35, INPUT_PULLUP);


  

 
}
 
void loop()
{

  // Run the FAT
  //fat.run();
  fat.debug();
  //test.update();
  //Serial.println(String("Btn: ") + (!test.read()));
    
}
