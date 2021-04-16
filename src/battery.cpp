#include <Arduino.h>
#include "battery.h"
#include "bluetooth.h"
#include "ledPanel.h"

// Battery timer global variables definitions
int batteryTimerFlag = 0;
bool chargerPluggedInFlag = false;
hw_timer_t * batteryTimer = NULL;

/* Battery timer setup */
void batteryTimerSetup()
{
    batteryTimer = timerBegin(0, 800, true); // setting prescaler to 800 makes timer tick 100,000 times per second 
    timerAttachInterrupt(batteryTimer, &onBatteryTimer, true);
    timerAlarmWrite(batteryTimer, 1000000, true); // alarm each 1,000,000/100,000 = 10 sec 
    timerAlarmEnable(batteryTimer);

    pinMode(CHARGER_IN_PIN, INPUT_PULLDOWN);
    attachInterrupt(CHARGER_IN_PIN, &onChargerPlug, RISING); //inverted 2 times 
}

 /* Battery check */
uint8_t showBatteryLevel()
{
    uint16_t analogValue = analogRead(BATTERY_CHARGE_PIN);
    uint8_t batteryLevel = (analogValue - 3000) * 0.0913242; // 0.0913242 = 100/1095

    
    Serial.print("Battery level = ");
    Serial.print(batteryLevel);
    Serial.print(" Analog value = ");
    Serial.println(analogValue);
    
    batteryTimerFlag = 0;

    BatteryLevelCharacteristic.setValue(&batteryLevel, 1);
    BatteryLevelCharacteristic.notify();
    return batteryLevel;
}

void IRAM_ATTR onBatteryTimer()
{
  Serial.println("Battery timer triggered");
  batteryTimerFlag = 1;
}

void IRAM_ATTR onChargerPlug()
{
  chargerPluggedInFlag = true;
}