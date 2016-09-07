#include <application.h>

#include "SunRiseLamp.h"
#include "neopixel.h"

void SunRiseLamp::begin(int totalTime, int pixelCount, int pixelPin, int pixelType)
{
  _pixelCount = pixelCount;
  _pixelPin = pixelPin;
  _pixelType = pixelType;
  pixelStringPtr = new Adafruit_NeoPixel(_pixelCount, _pixelPin, _pixelType);
  colorInterval = (totalTime * 1000) / (sizeof(colors)/sizeof(colors[0]));
  lastUpdate = millis();
  colorIndex = 0;
  pixelStringPtr->begin();
  pixelStringPtr->setBrightness(255);
  // pixelStringPtr->setPixelColor(0, 125,125,125);
  pixelStringPtr->show();
  delay(5000);
}

bool SunRiseLamp::update()
{
  bool updateLeds = false;
  int currentMillis = millis();
  if (currentMillis - lastUpdate > colorInterval)
  {
    if (direction == 1 && (colorIndex < sizeof(colors)/sizeof(colors[0]) -  1))
    {
      colorIndex++;
      updateLeds = true;
    }
    else if (direction == -1 && colorIndex > 0)
    {
      colorIndex--;
      updateLeds = true;
    }
    else
    {
      lastUpdate = currentMillis;
      return direction = 0;
    }
    lastUpdate = currentMillis;
  }
  if(updateLeds)
  {
    colorValues colorSet;
    colorSet.hexValue = colors[colorIndex];
    byte r = colorSet.ledValue[2];
    byte g = colorSet.ledValue[1];
    byte b = colorSet.ledValue[0];
    char colorString[32] = "";
    snprintf(colorString, sizeof(colorString), "Red:%x Green:%x Blue:%x", r, g, b);
    Serial.println(colorString);
    for (int i = 0; i < _pixelCount; i++)
    {
      pixelStringPtr->setPixelColor(i, r, g, b);
    }
    pixelStringPtr->show();
  }
  return true;
}

void SunRiseLamp::rise()
{

  colorIndex = 0;
  direction = 1;
}

void SunRiseLamp::set()
{

  colorIndex = sizeof(colors)/sizeof(colors[0]);
  direction = -1;
}
