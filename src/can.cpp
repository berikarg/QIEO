#include <Arduino.h>
#include "can.h"
#include "button.h"

RTC_DATA_ATTR int oxCanState = 0;
bool oxCanCheckFlag = false;

void oxCanSetup()
{
    pinMode(OX_CAN_PIN, INPUT_PULLDOWN);
    attachInterrupt(OX_CAN_PIN, oxCanISR, CHANGE);
    oxCanState = digitalRead(OX_CAN_PIN);
}

IRAM_ATTR void oxCanISR()
{
    oxCanCheckFlag = true;
    oxBtnTimerValue = 0;
    timerWrite(oxBtnTimer, oxBtnTimerValue); // reload oxygen button timer
}