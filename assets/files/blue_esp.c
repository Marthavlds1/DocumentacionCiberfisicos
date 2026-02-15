#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define LED_PIN 3
#define BTN_PIN 2

BLECharacteristic *pCharacteristic;

bool ledState = false;

class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    String value = pCharacteristic->getValue();

    if (value == "ON") {
      ledState = true;
      digitalWrite(LED_PIN, HIGH);
    }

    if (value == "OFF") {
      ledState = false;
      digitalWrite(LED_PIN, LOW);
    }
  }
};

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BTN_PIN, INPUT_PULLUP);

  BLEDevice::init("ESP32_LED");
  BLEServer *pServer = BLEDevice::createServer();

  BLEService *pService = pServer->createService("1234");

  pCharacteristic = pService->createCharacteristic(
                      "5678",
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_WRITE |
                      BLECharacteristic::PROPERTY_NOTIFY
                    );

  pCharacteristic->setCallbacks(new MyCallbacks());
  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->start();
}

void loop() {

  if (digitalRead(BTN_PIN) == LOW) {
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);

    if (ledState)
      pCharacteristic->setValue("LED_ON");
    else
      pCharacteristic->setValue("LED_OFF");

    pCharacteristic->notify();
    delay(300);
  }
}
