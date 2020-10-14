#include <Firmata.h>
#include "utility/firmataDebug.h"
#include <avr/wdt.h>
#include <aREST.h>

//#define SERIAL_DEBUG

// Create aREST instance
aREST rest = aREST();
bool isRebooted = false;



void setup()
{
  DEBUG_BEGIN(115200);
  Serial.begin(115200);

  isRebooted = true;

  rest.set_id("000004");
  rest.set_name("DFP");
  rest.variable("isRebooted",&isRebooted);

  rest.function("acknoledgeReboot", acknoledgeReboot);


  // Start watchdog
  wdt_enable(WDTO_8S);
 
}
 
void loop()
{

  rest.handle(Serial);
  wdt_reset();
    
}

// Handle acknoledge when client look arduino have rebooted
int acknoledgeReboot(String command) {
  DEBUG_PRINTLN("Acknoledge reboot");
  isRebooted = false;
  return 1;
}