#include "MAX30105.h"
#include "sensor.h"
#include "button.h"
#include "vibrationMotor.h"

PLX::PLX()
{
    ledBrightness = 0x7F; //Options: 0=Off to 255=50mA
    sampleAverage = SMP_AVE; //Options: 1, 2, 4, 8, 16, 32
    ledMode = 2; //Options: 1 = Red only, 2 = Red + IR
    sampleRate = SAMPLERATE; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
    pulseWidth = 215; //Options: 69, 118, 215, 411
    adcRange = 16384; //Options: 2048, 4096, 8192, 16384
}

void PLX::resetVariables()
{
    eIrAvg = 0;
    eRedAvg = 0;
    sumIrRMS = 0;
    sumRedRMS = 0;
    loopCnt = 0;
    eir = 0;
    eir_prev = 0;
    crosstime = 0;
    crosstime_prev = 0;

    HR = 70.0;
    SpO2 = 95.0;
}

void PLX::updateAvgAndRMS(double ir, double red)
{
    eIrAvg = eIrAvg * LPF_RATE + ir * (1 - LPF_RATE);    //average ir level by low pass filter
    eRedAvg = eRedAvg * LPF_RATE + red * (1 - LPF_RATE); //average red level by low pass filter
    sumIrRMS += (ir - eIrAvg) * (ir - eIrAvg);           //square sum of alternate component of IR level
    sumRedRMS += (red - eRedAvg) * (red - eRedAvg);      //square sum of alternate component of red level
}

void PLX::estimateHR(double ir)
{
    double bpm;

    eir = eir * IR_LPF_COEF + ir * (1.0 - IR_LPF_COEF); //estimated IR : low pass filtered IR signal
    if ( ((eir - eIrAvg) * (eir_prev - eIrAvg) < 0 ) && ((eir - eIrAvg) < 0.0))
    { //find zero cross at falling edge
        crosstime = millis();//system time in msec of falling edge
        if ( ((crosstime - crosstime_prev ) > (60 * 1000 / MAX_BPS)) && ((crosstime - crosstime_prev ) < (60 * 1000 / MIN_BPS)) ) 
        {
            bpm = 60.0 * 1000.0 / (double)(crosstime - crosstime_prev) ; //get bpm
            Serial.print("crossed instantaneous BPM = ");
            Serial.println(bpm);
            if (bpm < 1.6*getAvg(lastFiveHR, 5)) // hopefully filter out wrong values
            {
                HR = HR * HR_LPF_COEF + (1.0 - HR_LPF_COEF) * bpm;//estimated bpm by low pass filter
                for (int i = 0; i < 4; i++)
                    lastFiveHR[i] = lastFiveHR[i+1];
                lastFiveHR[4] = HR; 
            }
        }
        crosstime_prev = crosstime;
    }
    eir_prev = eir;
}

void PLX::estimateSpO2()
{
    double R;
    double iSpO2; // instantaneous SpO2
    R = (sqrt(sumRedRMS) / eRedAvg) / (sqrt(sumIrRMS) / eIrAvg);
    iSpO2 = -45.060 * R * R + 30.354 * R + 94.845;
    if(iSpO2 > MIN_SPO2)
        SpO2 = SPO2_LPF_COEF * SpO2 + (1.0 - SPO2_LPF_COEF) * iSpO2; //low pass filter
    sumRedRMS = 0;
    sumIrRMS = 0;
    loopCnt = 0;
    Serial.print("iSpO2 = "); Serial.print(iSpO2); Serial.print("; R = "); Serial.println(R);
}

double getAvg(double* arr, int arrSize)
{
    double runningSum = 0;
    for (int i = 0; i < arrSize; i++)
        runningSum += arr[i];
    return runningSum/arrSize;
}

