#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

#include "bluetooth.h"
#include "ledPanel.h"
#include "vibrationMotor.h"



bool _BLEClientConnected = false;

//HR service definitions
uint8_t HR_data[8] = {0b00000110, 0, 0, 0, 0, 0, 0, 0}; //8bit HR data , no extended data , defined in 0x2A37
BLECharacteristic HRMeasurementCharacteristics(BLEUUID((uint16_t)0x2A37), BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor HRDescriptor(BLEUUID((uint16_t)0x2901));

//SPo2 service definitions
uint8_t SpO2_data[20] = {0b00001000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0b00001000, 0, 0, 0}; // 20 byte SpO2 data
BLECharacteristic SpO2MeasurementCharacteristics(BLEUUID((uint16_t)0x2A5F), BLECharacteristic::PROPERTY_NOTIFY);
BLECharacteristic Spo2FeaturesCharacteristics(BLEUUID((uint16_t)0x2A60), BLECharacteristic::PROPERTY_READ);

//Battery level service definitions
BLECharacteristic BatteryLevelCharacteristic(BLEUUID((uint16_t)0x2A19), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);


class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      _BLEClientConnected = true;
      blueFlash();
      makeVibration();
    };
    void onDisconnect(BLEServer* pServer) {
      _BLEClientConnected = false;
    }
};

void initBLE() {
  //  Serial.println("Initializing...BLE as a heart rate monitor service 0x2A37");
  //  Serial.println("Smartphone application for smartphone can be used to monitor SpO2 as bpm");
  uint8_t SpO2_features[7] = {0b00000010, 0, 0, 0, 0, 0b00001000, 0};
  BLEDevice::init("QIEO"); //Device name
  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  // Create the HR BLE Service
  BLEService *pHR = pServer->createService(HRService);
  pHR->addCharacteristic(&HRMeasurementCharacteristics);
  HRDescriptor.setValue("use % instead of BPM");
  HRMeasurementCharacteristics.addDescriptor(&HRDescriptor);
  HRMeasurementCharacteristics.addDescriptor(new BLE2902());

  // Create the SPO2 BLE Service
  BLEService *pSpO2 = pServer->createService(SpO2Service);
  pSpO2->addCharacteristic(&SpO2MeasurementCharacteristics);
  pSpO2->addCharacteristic(&Spo2FeaturesCharacteristics);
  SpO2MeasurementCharacteristics.addDescriptor(new BLE2902());

  //Create the Battery Level BLE Service
  BLEService *pBat = pServer->createService(BatteryService);
  pBat->addCharacteristic(&BatteryLevelCharacteristic);
  BatteryLevelCharacteristic.addDescriptor(new BLE2902());


  pServer->getAdvertising()->addServiceUUID(HRService);
  pServer->getAdvertising()->addServiceUUID(SpO2Service);
  pServer->getAdvertising()->addServiceUUID(BatteryService);
  pHR->start();
  pSpO2->start();
  pBat->start();
  pServer->getAdvertising()->start();// Start advertising
  Spo2FeaturesCharacteristics.setValue(SpO2_features, 7);
}

void sendBLEData(double Ebpm, double eSpO2)
{
  /*
  if ( ir < FINGER_ON) 
  {
  //eSpO2 = MIN_SPO2; //indicator for finger detached
  HR_data[0] = 0b0000100; // set Sensor Contact Status bit to 0
  SpO2_data[16] = 0b00000000;
  }
  else
  {
    HR_data[0] = 0b0000110; // set Sensor Contact Status bit to 1
    SpO2_data[16] = 0b00001000;
  }
  */

  HR_data[1] = (uint8_t)Ebpm;
  HR_data[2] =  0x00;

  HRMeasurementCharacteristics.setValue(HR_data, 2);
  HRMeasurementCharacteristics.notify();

  SpO2_data[1] = (uint8_t)eSpO2;
  SpO2_data[2] = 0x00;
  SpO2_data[3] = (uint8_t)Ebpm;
  SpO2_data[4] = 0x00;
  SpO2MeasurementCharacteristics.setValue(SpO2_data, 20);
  SpO2MeasurementCharacteristics.notify();  
}
