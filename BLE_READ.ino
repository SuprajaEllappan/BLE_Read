#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

// UUIDs for the service and characteristics
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC1_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a1"
#define CHARACTERISTIC2_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a2"
#define CHARACTERISTIC3_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a3"

// Declare characteristic pointers
BLECharacteristic *pCharacteristic1;
BLECharacteristic *pCharacteristic2;
BLECharacteristic *pCharacteristic3;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  // Initialize BLE
  BLEDevice::init("MyESP32");

  // Create a BLE server
  BLEServer *pServer = BLEDevice::createServer();

  // Create a BLE service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create BLE characteristics
  pCharacteristic1 = pService->createCharacteristic(
                                         CHARACTERISTIC1_UUID,
                                         BLECharacteristic::PROPERTY_NOTIFY
                                       );
  pCharacteristic1->addDescriptor(new BLE2902());  // Required for notifications

  pCharacteristic2 = pService->createCharacteristic(
                                         CHARACTERISTIC2_UUID,
                                         BLECharacteristic::PROPERTY_NOTIFY 
                                       );
  pCharacteristic2->addDescriptor(new BLE2902());  // Required for notifications

  pCharacteristic3 = pService->createCharacteristic(
                                         CHARACTERISTIC3_UUID,
                                         BLECharacteristic::PROPERTY_NOTIFY 
                                       );
  pCharacteristic3->addDescriptor(new BLE2902());  // Required for notifications

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // Functions to help with iPhone connection issues
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();

  Serial.println("Characteristic defined! Now you can read it on your phone!");
}

void loop() {
  // Notify characteristic1 with a string value
  String value = "Hello, nRF Connect!";
  pCharacteristic1->setValue(value.c_str());  // Convert String to std::string
  pCharacteristic1->notify();

  // Notify characteristic2 with a single character value
  char singleCharValue = 'A';
  pCharacteristic2->setValue((uint8_t*)&singleCharValue, 1);// Cast to uint8_t*
  pCharacteristic2->notify();

  // Notify characteristic3 with a random integer value as a string
  int tx_value = random(-1, 100);
  char tx_string[8];
  dtostrf(tx_value, 1, 2, tx_string);
  pCharacteristic3->setValue(tx_string);
  pCharacteristic3->notify();

  delay(2000);  // Wait for 2 seconds before the next notification
}
