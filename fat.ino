#include <Firmata.h>
#include "utility/firmataDebug.h"
#include "Settings.h"
#include "Fat.h"
#include <avr/wdt.h>
#include <aREST.h>

//#define SERIAL_DEBUG


// Create FAT object
Fat fat = Fat();
bool isRebooted;
bool isWashed;
bool isAuto;
bool isStopped;
bool isSecurity;
bool isEmergencyStopped;
// Create aREST instance
aREST rest = aREST();



void setup()
{
  DEBUG_BEGIN(115200);
  Serial.begin(115200);

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
  fat.setWashingDurationInSecond(13);

  // Set the time to wait between two wash
  fat.setWaitTimeBetweenWasingInSecond(30);

  // Set the time to wait after pump run and before barrel move
  fat.setWaitTimePumpInSecond(2);

  // Set the time to wait before start clean cycle to avoid captor issue
  fat.setWaitTimeForceWashingCycleInMinute(180);

  // Set the time to wait before start clean cycle when tempeture is under than 0.
  fat.setWaitTimeForceWashingCycleFreezeInMinute(60);

  // Configure aREST
  isRebooted = true;
  isWashed = false;
  isAuto = false;
  isStopped = false;
  isSecurity = false;
  isEmergencyStopped = false;

  rest.set_id("000004");
  rest.set_name("DFP");
  rest.variable("isRebooted",&isRebooted);
  rest.variable("isWashed",&isWashed);
  rest.variable("isAuto",&isAuto);
  rest.variable("isStopped",&isStopped);
  rest.variable("isSecurity",&isSecurity);
  rest.variable("isEmergencyStopped",&isEmergencyStopped);

  rest.function("acknoledgeReboot", acknoledgeReboot);
  rest.function("action", action);

  // Start watchdog
  wdt_enable(WDTO_4S);
 
}
 
void loop()
{

  #ifdef SERIAL_DEBUG
  fat.debug();
  #endif

  fat.run();
  updateState();
  rest.handle(Serial);
  wdt_reset();
    
}

// Handle acknoledge when client look arduino have rebooted
int acknoledgeReboot(String command) {
  DEBUG_PRINTLN("Acknoledge reboot");
  isRebooted = false;
  return 1;
}

// Handle action on DFP
int action(String command) {
  if(command == "auto") {
    fat.autoMode();
  }
  else if(command == "stop") {
    fat.stop();
  }
  else if(command == "wash") {
    fat.wash();
  }
  else if(command == "startBarrel") {
    fat.startMotorBarrel();
  }
  else if(command == "stopBarrel") {
    fat.stopMotorBarrel();
  }
  else if(command == "startPump") {
    fat.startMotorPump();
  }
  else if(command == "stopPump") {
    fat.stopMotorPump();
  }

  return 0;
}

// Update the current state of DFP for API
void updateState() {
  State state = fat.getState();
  isWashed = state.isWashed;
  isAuto = state.isAuto;
  isStopped = state.isStopped;
  isSecurity = state.isSecurity;
  isEmergencyStopped = state.isEmergencyStopped;
}