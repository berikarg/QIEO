#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

#define FINGER_ON 50000 // if ir signal is lower than this , it indicates your finger is not on the sensor

extern bool _BLEClientConnected;
//HR service declarations
extern uint8_t HR_data[8]; //8bit HR data , no extended data , defined in 0x2A37
#define HRService BLEUUID((uint16_t)0x180D)
extern BLECharacteristic HRMeasurementCharacteristics;
extern BLEDescriptor HRDescriptor;

//SPo2 service declarations
extern uint8_t SpO2_data[20]; // 20 byte SpO2 data
#define SpO2Service BLEUUID((uint16_t)0x1822)
extern BLECharacteristic SpO2MeasurementCharacteristics;
extern BLECharacteristic Spo2FeaturesCharacteristics;

//Battery level service declarations
#define BatteryService BLEUUID((uint16_t)0x0180F)
extern BLECharacteristic BatteryLevelCharacteristic;



void initBLE();
void sendBLEData(double Ebpm, double eSpO2);
#endif