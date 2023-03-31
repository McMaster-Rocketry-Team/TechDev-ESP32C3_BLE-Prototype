#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// Define the UUID for the BLE service and characteristic
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"  
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"  

#define TRIGGER_PIN 5   
#define ECHO_PIN 18     

BLECharacteristic *pCharacteristic;   

void setup() {
  Serial.begin(115200);   

  pinMode(TRIGGER_PIN, OUTPUT);   
  pinMode(ECHO_PIN, INPUT);       

  BLEDevice::init("ESP32-BLE-Server");   // Initialize the BLE device
  BLEServer *pServer = BLEDevice::createServer();   // Create the BLE server
  BLEService *pService = pServer->createService(SERVICE_UUID);   // Create the BLE service
  pCharacteristic = pService->createCharacteristic(
                       CHARACTERISTIC_UUID,
                       BLECharacteristic::PROPERTY_READ |   // Set the characteristic property as readable and writable
                       BLECharacteristic::PROPERTY_WRITE   // Set the characteristic property as writable
                     );     
                                       
  pCharacteristic->setValue("Hello, World!");  
  pService->start();   // Start the BLE service
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();   // Get the advertising object
  pAdvertising->addServiceUUID(SERVICE_UUID);   // Add the service UUID to the advertising data
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);   // Set the minimum preferred connection interval
  pAdvertising->setMinPreferred(0x12);   // Set the maximum preferred connection interval
  BLEDevice::startAdvertising();   // Start advertising the BLE service
  Serial.println("Characteristic defined");   // Print a message to indicate that the characteristic has been defined
}

void loop() {
  float distance = getDistance();    
  String distanceStr = String(distance);   
  pCharacteristic->setValue(distanceStr.c_str());  
  delay(10);   
}

float getDistance(){
    digitalWrite(TRIGGER_PIN, LOW);    
    delayMicroseconds(2);   
    digitalWrite(TRIGGER_PIN, HIGH);   
    delayMicroseconds(10);  
    digitalWrite(TRIGGER_PIN, LOW);    
    float duration = pulseIn(ECHO_PIN, HIGH);    
    float distance = duration * 0.034 / 2;  
    return distance;    
}
