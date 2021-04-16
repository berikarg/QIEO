#include <Arduino.h>
#include "vibrationMotor.h"

// Vibration timer global variables definitions
hw_timer_t * vibrationTimer = NULL;

/* Vibration motor setup */
void vibrationMotorSetup()
{
    vibrationTimer = timerBegin(1, 800, true); // setting prescaler to 800 makes timer tick 100,000 times per second 
    timerAttachInterrupt(vibrationTimer, &onVibrationTimer, true);
    timerAlarmWrite(vibrationTimer, 20000, false); // alarm each 20000/100,000 = 200 msec
    timerAlarmEnable(vibrationTimer);
    pinMode(VIBR_PIN, OUTPUT);
}

void IRAM_ATTR onVibrationTimer()
{
  digitalWrite(VIBR_PIN, LOW);
}

void makeVibration()
{
    digitalWrite(VIBR_PIN, HIGH);
    timerRestart(vibrationTimer);
    timerAlarmEnable(vibrationTimer);
}