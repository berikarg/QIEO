#include <Arduino.h>
#include "button.h"
#include "MAX30105.h"
#include "sensor.h"
#include "battery.h"
#include "vibrationMotor.h"
#include "deepSleep.h"
#include "ledPanel.h"

// Global variable definitions
int btnCNTR = 0; // counts button presses
hw_timer_t * oxBtnTimer = NULL;// Oxygen button timer
RTC_DATA_ATTR uint64_t oxBtnTimerValue = 0; // to store Oxygen button timer value through deepsleep

void btnSetup(){
    pinMode(GP_BTN, INPUT);
    attachInterrupt(GP_BTN, btnISR, RISING);
    pinMode(OX_BTN, INPUT);
    attachInterrupt(OX_BTN, oxBtnISR,CHANGE);

    oxBtnTimer = timerBegin(3, 8000, true); // setting prescaler to 8000 makes timer tick 10,000 times per second 
    timerStop(oxBtnTimer);
    timerWrite(oxBtnTimer, oxBtnTimerValue);
}

void IRAM_ATTR btnISR()
{
  btnCNTR++;
  Serial.print("Button count from ISR = ");
  Serial.println(btnCNTR);
  // reload deep sleep timer
  timerRestart(deepSleepTimer);
  timerAlarmEnable(deepSleepTimer);
}

// Counts how long oxygen button was pressed
void IRAM_ATTR oxBtnISR()
{
  if (digitalRead(OX_BTN))
    timerStart(oxBtnTimer);
  else
    timerStop(oxBtnTimer);
}


void readBtnPresses()
{
  if(btnCNTR > 0)
  {
    unsigned long first_pressed_millis = 0;
    bool btnDone = false;
    first_pressed_millis = millis();
    // wait for user to press the button again
    while (millis() - first_pressed_millis < BTN_MAX_DELAY)
    {
        // if button pressed again
        if (btnCNTR > 1)
        {
            if(!qieoPLX.sleepFlag) // if sensor is ON, turn sensor OFF
            {
              qieoPLX.shutDown();
              qieoPLX.sleepFlag = true;
            }
            else if(qieoPLX.sleepFlag) //else if sensor is OFF, turn sensor ON
            {
                qieoPLX.wakeUp();
                qieoPLX.setup(qieoPLX.ledBrightness, qieoPLX.sampleAverage, qieoPLX.ledMode, qieoPLX.sampleRate, qieoPLX.pulseWidth, qieoPLX.adcRange); //Configure sensor with these settings

                //reset global SpO2 and HR
                qieoPLX.resetVariables();
                qieoPLX.sleepFlag = false;
                qieoPLX.firstReading = true;
            }
            btnDone = true;
            break;
        }
    }
    // if on timeout no button pressed it means the button pressed only one time
    if (!btnDone)
    {
      uint8_t batteryLevel = showBatteryLevel();
      showBatteryLedPanel(batteryLevel);
      //batteryChargeStartIndication();
      Serial.print("Oxygen timer value = ");
      oxBtnTimerValue = timerRead(oxBtnTimer);
      delay(100);
      Serial.println((uint32_t)oxBtnTimerValue);
    }
    btnCNTR = 0;
  }
}
