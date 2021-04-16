#ifndef DEEPSLEEP_H
#define DEEPSLEEP_H

#define TIME_TO_SLEEP 300 //in seconds 300
#define WAKEUP_PIN_BITMASK 0x0B00000000 // pins 32,33,35 are enabled
//#define WAKEUP_PIN_BITMASK 0x1B00000000 // pins 32,33,35,36 are enabled
extern hw_timer_t * deepSleepTimer;

void IRAM_ATTR onDeepSleepTimer();
void deepSleepSetup();

#endif