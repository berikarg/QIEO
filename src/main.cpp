#include <Arduino.h>
#include <Wire.h>

#include "main.h"
#include "MAX30105.h"
#include "sensor.h"
#include "battery.h"
#include "bluetooth.h"
#include "button.h"
#include "vibrationMotor.h"
#include "deepSleep.h"
#include "ledPanel.h"
#include "can.h"

PLX qieoPLX;

void setup()
{
    Serial.begin(115200);
    Serial.println("Initializing...");
    // Initialize sensor
    if (!qieoPLX.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
    {
        Serial.println("MAX3010X was not found.");
    }
    qieoPLX.setup(qieoPLX.ledBrightness, qieoPLX.sampleAverage, qieoPLX.ledMode, qieoPLX.sampleRate, qieoPLX.pulseWidth, qieoPLX.adcRange); //Configure sensor with these settings
    qieoPLX.shutDown();
    qieoPLX.sleepFlag = true;

    //Start BLE
    initBLE();

    // General Purpose Button setup
    btnSetup();

    // battery timer setup
    batteryTimerSetup();

    // vibration motor setup
    vibrationMotorSetup();

    //oxCan setup
    oxCanSetup();

    // Deep Sleep
    deepSleepSetup();

    //LED panel setup
    ledSetup();

    //check if some inputs have been entered
    if (digitalRead(OX_BTN))
        timerStart(oxBtnTimer);

    if (digitalRead(CHARGER_IN_PIN))
        chargerPluggedInFlag = true;
} //setup

void loop()
{
    readBtnPresses(); // check if user pressed the button
    if(!qieoPLX.sleepFlag)
    {
        if(qieoPLX.firstReading)
        {
            if(checkFinger())
                checkPLX();
        }
        else
            checkPLX();
    }

    if(batteryTimerFlag)
    {
        showBatteryLevel();
    }
    if(chargerPluggedInFlag)
    {
        Serial.println("Charger has been plugged in");
        batteryChargeStartIndication();
        chargerPluggedInFlag = false;
    }
} // main loop

void checkPLX()
{
        
    if(qieoPLX.firstReading)
    {
        makeVibration();
        qieoPLX.firstReading = false;
    }

    qieoPLX.check(); //poll sensor for new data
    while (qieoPLX.available())
    {
    qieoPLX.loopCnt++;
    double red = qieoPLX.getIR(); // chinese manufacturer is a rediska
    double ir = qieoPLX.getRed();

    // abort if finger is not attached or button is pressed
    if(btnCNTR > 0 || ir < FINGER_ON){
        Serial.print("Finger is not found. Turning OFF, IR value = ");
        Serial.println(ir);
        //turn the sensor OFF
        qieoPLX.sleepFlag = true;
        qieoPLX.shutDown();
        makeVibration();
        btnCNTR = 0;
        return;
    }

    qieoPLX.updateAvgAndRMS(ir, red);
    qieoPLX.estimateHR(ir);
    if((qieoPLX.loopCnt % SPO2_INTERVAL) == 0)
    {
        qieoPLX.estimateSpO2();
        Serial.print("Avg SpO2: ");
        Serial.print(qieoPLX.SpO2);
        Serial.print(" HR: ");
        Serial.println(qieoPLX.HR);

        if(qieoPLX.SpO2 < MAX_SPO2 && qieoPLX.SpO2 > MIN_SPO2)
        {
            showSpO2LedPanel(qieoPLX.SpO2);
            sendBLEData(qieoPLX.HR, qieoPLX.SpO2);
        }
    }
    qieoPLX.nextSample();
    }
}

// checks if finger is attached.
// Waits for 5 sec
// Returns false if a button is pressed or 
// time run out
// !TODO maybe combine if and else if parts as they do the same
bool checkFinger()
{
    unsigned long btnPressTime = millis();
    // do not start before finger is attached, wait 5 sec before
    while(qieoPLX.getIR() < 50000)
    {
        Serial.print("Finger is not detected. IR value = ");
        Serial.println(qieoPLX.getIR());
        delay(100); // no need to check if user attached finger too often
        // abort if 5 sec has passed
        if(millis() - btnPressTime > 5000)
        {
            Serial.println("No contact for 5 sec. Turning OFF");
            qieoPLX.sleepFlag = true;
            qieoPLX.shutDown();
            makeVibration();
            return false;
        }
        // turn the sensor OFF if a button is pressed
        else if(btnCNTR > 0)
        {
            qieoPLX.sleepFlag = true;
            qieoPLX.shutDown();
            Serial.println("Abort pulseoximeter...");
            makeVibration();
            btnCNTR = 0;
            return false;
        }
    }
    return true;
}