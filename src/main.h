#ifndef MAIN_H
#define MAIN_H

//HARDWARE DEFINITION
#define WAKEUP_SLEEP 33
#define SDA_PIN 21
#define SCL_PIN 22

void setup();
void loop();
void checkPLX();
bool checkFinger();

#endif