#ifndef CAN_H
#define CAN_H

#define OX_CAN_PIN GPIO_NUM_13

void oxCanSetup();
IRAM_ATTR void oxCanISR();
extern RTC_DATA_ATTR int oxCanState;

#endif