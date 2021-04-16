#ifndef BUTTON_H
#define BUTTON_H


#define GP_BTN GPIO_NUM_34
#define OX_BTN GPIO_NUM_35
#define BTN_MAX_DELAY 400 //time given to user for second press

extern int btnCNTR; // counts button presses
extern hw_timer_t * oxBtnTimer;
extern RTC_DATA_ATTR uint64_t oxBtnTimerValue;

void btnSetup();
void IRAM_ATTR btnISR();
void readBtnPresses();
void IRAM_ATTR oxBtnISR();

#endif