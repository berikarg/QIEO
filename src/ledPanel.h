#ifndef LEDPANEL_H
#define LEDPANEL_H

#include "FastLED.h"

#define LED_CONTROL_PIN     19
#define NUM_LEDS            16
extern CRGB leds[NUM_LEDS];

void ledSetup();
void svetofor();
void showBatteryLedPanel(uint8_t batteryLevel);
void turnPanelOff();
void showSpO2LedPanel(double SpO2);
void blueFlash();
void batteryChargeStartIndication();

#endif