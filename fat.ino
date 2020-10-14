#include "settings.h"
#include "timer.h"
#include <Firmata.h>
#include "utility/firmataDebug.h"
#include <avr/wdt.h>
#include <aREST.h>
#include <DS18B20.h>
#include "DHTesp.h"

//#define SERIAL_DEBUG

// Create aREST instance
aREST rest = aREST();
bool isRebooted = false;
DS18B20 ds(PIN_DS18B20);
DHTesp dht = DHTesp();
int waterTemp = 0;
int airTemp = 0;
int airHumidity = 0;
Timer timer;


void setup()
{
  DEBUG_BEGIN(115200);
  Serial.begin(115200);

  isRebooted = true;

  rest.set_id("000004");
  rest.set_name("DFP");
  rest.variable("isRebooted",&isRebooted);
  rest.variable("waterTemp", &waterTemp);
  rest.variable("airTemp", &airTemp);
  rest.variable("airHumidity", &airHumidity);

  rest.function("acknoledgeReboot", acknoledgeReboot);

  timer = Timer();
  timer.setTimerInSecond(10);
  timer.start();
  dht.setup(PIN_DHT, DHTesp::DHT11);
  readCaptor();


  // Start watchdog
  wdt_enable(WDTO_8S);
 
}
 
void loop()
{
  timer.update();
  if(timer.isJustFinished()) {
    readCaptor();
    timer.start();
  }
  rest.handle(Serial);
  wdt_reset();
    
}

// Handle acknoledge when client look arduino have rebooted
int acknoledgeReboot(String command) {
  DEBUG_PRINTLN("Acknoledge reboot");
  isRebooted = false;
  return 1;
}

void readCaptor() {
  waterTemp = ds.getTempC();
  airTemp = (int)dht.getTemperature();
  airHumidity = (int)dht.getHumidity();
  DEBUG_PRINT("WaterTemp:");
  DEBUG_PRINTLN(waterTemp);
  DEBUG_PRINT("AirTemp");
  DEBUG_PRINTLN(airTemp);
  DEBUG_PRINT("AirHumidity");
  DEBUG_PRINTLN(airHumidity);
}
