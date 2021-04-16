#ifndef BATTERY_H
#define BATTERY_H

#define BATTERY_CHARGE_PIN GPIO_NUM_33
#define CHARGER_IN_PIN GPIO_NUM_32
#define BATTERY_CHECK_INTERVAL 60

// Battery timer global variables declarations
extern int batteryTimerFlag;
extern bool chargerPluggedInFlag;
extern hw_timer_t * batteryTimer;

uint8_t showBatteryLevel();
void batteryTimerSetup();
void IRAM_ATTR onBatteryTimer();
void IRAM_ATTR onChargerPlug();

#endif