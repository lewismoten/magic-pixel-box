#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

// If any bluetooth settings change you may need to forget and pair the device to clear cached settings
// ie - id's, services, descriptors, characteristics, etc.

#define SERVICE_NAME "My Service Name"
#define SERVICE_ID "4c657769-7320-4d6f-7465-6e0000000001"

#define VALUE_DESCRIPTOR_TEXT "Gets the value"
#define VALUE_DESCRIPTOR_ID "4c657769-7320-4d6f-7465-6e0000000002"
#define VALUE_CHARACTERISTIC_ID "4c657769-7320-4d6f-7465-6e0000000003"

String _value = "My Original Value";

BLEDescriptor myValueDescriptor(VALUE_DESCRIPTOR_ID);
BLECharacteristic myValueCharacteristic(VALUE_CHARACTERISTIC_ID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);

bool _connected = false;
class ServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* server) {
      _connected = true;
      Serial.println("connected");
    };
    void onDisconnect(BLEServer* server) {
      _connected = false;
      Serial.println("disconnected");
    }
};

void initializeBluetoothLowEnergyService() {
  BLEDevice::init(SERVICE_NAME);
  BLEServer *myServer = BLEDevice::createServer();
  myServer->setCallbacks(new ServerCallbacks());
  BLEService *myService = myServer->createService(SERVICE_ID);

  myService->addCharacteristic(&myValueCharacteristic);
  myValueDescriptor.setValue(VALUE_DESCRIPTOR_TEXT);
  myValueCharacteristic.addDescriptor(&myValueDescriptor);
  myValueCharacteristic.addDescriptor(new BLE2902());

  myServer->getAdvertising()->addServiceUUID(SERVICE_ID);
  myService->start();
  myServer->getAdvertising()->start();
  myValueCharacteristic.setValue(_value.c_str());
}

void setup() {
  Serial.begin(115200);
  initializeBluetoothLowEnergyService();
}

void loop() {
  String currentValue = myValueCharacteristic.getValue().c_str();
  if (currentValue != _value) {
    _value = currentValue;
    Serial.print("New Value: " + currentValue);
  }
  delay(1000);
}
