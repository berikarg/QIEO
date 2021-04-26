#ifndef SENSOR_H
#define SENSOR_H

//CUSTOM DEFINITION
#define LPF_RATE 0.95    // low pass filter coefficient
#define IR_LPF_COEF 0.85 //IR filter coefficient to remove notch, should be smaller than LPF_RATE
#define HR_LPF_COEF 0.90 // low pass filter coefficient for HRM in bpm
#define SPO2_LPF_COEF 0.70 // low pass filter coefficient for HRM in bpm

#define MAX_SPO2 100.0
#define MIN_SPO2 85.0

#define FINGER_ON 50000 // if ir signal is lower than this , it indicates your finger is not on the sensor
#define MAX_BPS 180
#define MIN_BPS 45

#define SAMPLERATE 800
#define SMP_AVE 4 // sample averaging value
#define HR_AVE 4 // averaging bpm readings
#define SPO2_INTERVAL 400 // how often to calculate spo2

class PLX: public MAX30105
{
private:
    double eIrAvg;
    double eRedAvg;
    double sumIrRMS;
    double sumRedRMS;
    uint32_t crosstime;
    uint32_t crosstime_prev;
    double eir;
    double eir_prev;
    double lastFiveHR[5] = {70,70,70,70,70};

public:
    byte ledBrightness; //Options: 0=Off to 255=50mA
    byte sampleAverage; //Options: 1, 2, 4, 8, 16, 32
    byte ledMode; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
    int sampleRate; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
    int pulseWidth; //Options: 69, 118, 215, 411
    int adcRange; //Options: 2048, 4096, 8192, 16384

    bool sleepFlag;         // sensor flag
    bool firstReading;
    double HR;              //overall HR that is returned to user
    double SpO2;            //overall SpO2 that is returned to user
    int loopCnt;

    PLX();
    void init();
    void resetVariables();
    void updateAvgAndRMS(double ir, double red);
    void estimateHR(double ir);
    void estimateSpO2();
};

double getAvg(double* arr, int arrSize);

extern PLX qieoPLX;

#endif