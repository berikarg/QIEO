#include <Arduino.h>
#include "button.h"
#include "battery.h"
#include "can.h"
#include "deepSleep.h"

// Deep Sleep timer global variables definitions
hw_timer_t * deepSleepTimer = NULL;

void deepSleepSetup()
{
    deepSleepTimer = timerBegin(2, 8000, true); // setting prescaler to 8000 makes timer tick 10,000 times per second 
    timerAttachInterrupt(deepSleepTimer, &onDeepSleepTimer, true);
    timerAlarmWrite(deepSleepTimer, TIME_TO_SLEEP*10000, false); // alarm each TIME_TO_SLEEP sec
    timerAlarmEnable(deepSleepTimer);

    if(oxCanState)
    {
      esp_sleep_enable_ext0_wakeup(OX_CAN_PIN, 0); // Configure the control button as a wake up source
    }
    else
    {
      esp_sleep_enable_ext0_wakeup(OX_CAN_PIN, 1); // Configure the control button as a wake up source
    }
    

    //esp_sleep_enable_ext0_wakeup(OX_CAN_PIN, 0); // Configure the control button as a wake up source
    esp_sleep_enable_ext1_wakeup(WAKEUP_PIN_BITMASK, ESP_EXT1_WAKEUP_ANY_HIGH);// Configure PINS 32,33,35,36 as wake up sources
}

void IRAM_ATTR onDeepSleepTimer()
{
  oxBtnTimerValue = timerRead(oxBtnTimer);
  esp_deep_sleep_start();
}