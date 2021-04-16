#ifndef VIBRATIONMOTOR_H
#define VIBRATIONMOTOR_H

#define VIBR_PIN GPIO_NUM_16

// Vibration timer global variables declarations
extern hw_timer_t *vibrationTimer;

void vibrationMotorSetup();
void IRAM_ATTR onVibrationTimer();
void makeVibration();

#endif