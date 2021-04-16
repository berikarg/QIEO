#ifndef DEEPSLEEP_H
#define DEEPSLEEP_H

#define TIME_TO_SLEEP 300 //in seconds 300
#define WAKEUP_PIN_BITMASK 0x0D00000000 // pins 32,34,35 are enabled
extern hw_timer_t * deepSleepTimer;

void IRAM_ATTR onDeepSleepTimer();
void deepSleepSetup();

#endif