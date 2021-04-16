#include <ESP32Ticker.h>
#include "FastLED.h"
#include "ledPanel.h"

Ticker tickerTurnLedOff;

CRGB leds[NUM_LEDS];

void ledSetup()
{
  FastLED.addLeds<WS2812, LED_CONTROL_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
}

void svetofor()
{
  for (int i = 0; i < NUM_LEDS; i++)
      leds[i] = CRGB(255, 0, 0);
  FastLED.show();
  delay(500);
  for (int i = 0; i < NUM_LEDS; i++)
      leds[i] = CRGB(255, 0, 0);
  FastLED.show();
  delay(500);
  for (int i = 0; i < NUM_LEDS; i++)
      leds[i] = CRGB(255, 0, 0);
  FastLED.show();
  delay(500);
  FastLED.clear(true);
}

void showBatteryLedPanel(uint8_t batteryLevel)
{
  FastLED.clear(true);
  if(batteryLevel >= 0 && batteryLevel < 34)
  {
    for (int i = 0; i < NUM_LEDS; i++)
      leds[i] = CRGB(255, 0, 0);
  }
  else if(batteryLevel >= 34 && batteryLevel < 67)
  {
    for (int i = 0; i < NUM_LEDS; i++)
      leds[i] = CRGB(180, 255, 0);
  }
  else if(batteryLevel >= 67 && batteryLevel <= 100)
  {
    for (int i = 0; i < NUM_LEDS; i++)
      leds[i] = CRGB(0, 255, 0);
  }

  FastLED.show();
  tickerTurnLedOff.once(2, turnPanelOff);
}

void turnPanelOff()
{
  FastLED.clear(true);
}

void showSpO2LedPanel(double SpO2)
{
  FastLED.clear(true);
  if(SpO2 < 95)
  {
    for (int i = 0; i < NUM_LEDS; i++)
      leds[i] = CRGB(255, 0, 0);
  }
  else if (SpO2 < 97)
  {
    for (int i = 0; i < NUM_LEDS; i++)
      leds[i] = CRGB(255, 255, 0);
  }
  else
  {
    for (int i = 0; i < NUM_LEDS; i++)
      leds[i] = CRGB(0, 255, 0);
  }
  FastLED.show();
  tickerTurnLedOff.once(2, turnPanelOff);
}

void blueFlash()
{
  FastLED.clear(true);
  for (int i = 0; i < NUM_LEDS; i++)
      leds[i] = CRGB(0, 0, 255);
  FastLED.show();
  tickerTurnLedOff.once(2, turnPanelOff);
}

void batteryChargeStartIndication()
{
  FastLED.clear(true);
  for (int j = 0; j < 3; j++)
  {
    for (int i = NUM_LEDS - 1; i > -1; i--)
    {
      delay(50);
      leds[i] = CRGB(0, 136, 0);
      FastLED.show(); 
    }
    delay(50);
    FastLED.clear(true);
  }
}