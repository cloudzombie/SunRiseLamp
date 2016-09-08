// This #include statement was automatically added by the Particle IDE.
#include "Libraries/DailyTimer.h"

// Created by Jean-Pierre Figaredo

#include "application.h"
#include "Libraries/neopixel.h"
#include "SunRiseLamp/SunRiseLamp.h"

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

// IMPORTANT: Set pixel COUNT, PIN and TYPE
#define PIXEL_PIN D6
#define PIXEL_COUNT 24          //<<<<<<<<<<< check here
#define PIXEL_TYPE WS2812B     //<<<<<<<<<<< check here
#define PIXEL_BRIGHTNESS 100

//#define SUN_RISE_HOUR 15
//#define SUN_SET_HOUR 0

#define Sun_RISE_LENGTH_IN_SEC 300  // Set this for how many seconds you wish to take to rise or set

SunRiseLamp sunriseLeds;
int lastHour = 24;

DailyTimer weekdayTimer(0, 41, 0, 46, WEEKDAYS);  //(startHour, startMinute, endHour, endMinute, DayOfWeek)
DailyTimer weekendTimer(7, 30, 19, 30, WEEKENDS);

void setup()
{
  Particle.function("test", testSunMachine);
  Serial.begin(9600);
  Time.zone(-7);
  Particle.syncTime();
  weekendTimer.begin();
  weekdayTimer.begin();
  sunriseLeds.begin(Sun_RISE_LENGTH_IN_SEC, PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);
}


void loop()
{
  sunriseLeds.update();
  if(weekdayTimer.startTrigger() || weekendTimer.startTrigger())
  {
    sunriseLeds.rise();
  }
  if(weekdayTimer.endTrigger() || weekendTimer.endTrigger())
  {
    sunriseLeds.set();
  }
}

int testSunMachine(const char* data)
{
  if(strstr(data, "set"))
  {
    Serial.println("Manually told to Set");
    sunriseLeds.set();
    return 0;
  }
  else if(strstr(data, "rise"))
  {
    Serial.println("Manually told to Rise");
    sunriseLeds.rise();
    return 1;
  }
  return -1;
}
