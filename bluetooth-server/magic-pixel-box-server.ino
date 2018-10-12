#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

#define SERVICE_NAME "My Service Name"
#define SERVICE_ID "96e75c51-3361-49e4-9e4e-a5b270b3bc41"

#define GET_VALUE_DESCRIPTOR_TEXT "Gets the value"
#define GET_VALUE_DESCRIPTOR_ID "8d08aac3-89d6-4051-8e9c-ba43036813e2"
#define GET_VALUE_CHARACTERISTIC_ID "8418b872-c609-4420-925b-292557fb9e73"

#define SET_VALUE_DESCRIPTOR_TEXT "Sets the value"
#define SET_VALUE_DESCRIPTOR_ID "673e6ee0-8e29-4189-bfeb-0763de92701e"
#define SET_VALUE_CHARACTERISTIC_ID "e4662683-ac03-4b4c-b1b3-3fba451d3356"

BLEDescriptor getValueDescriptor(GET_VALUE_DESCRIPTOR_ID);
BLECharacteristic getValueCharacteristic(GET_VALUE_CHARACTERISTIC_ID, BLECharacteristic::PROPERTY_READ);
BLEDescriptor setValueDescriptor(SET_VALUE_DESCRIPTOR_ID);
BLECharacteristic setValueCharacteristic(SET_VALUE_CHARACTERISTIC_ID, BLECharacteristic::PROPERTY_WRITE);

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

  myService->addCharacteristic(&getValueCharacteristic);
  getValueDescriptor.setValue(GET_VALUE_DESCRIPTOR_TEXT);
  getValueCharacteristic.addDescriptor(&getValueDescriptor);
  getValueCharacteristic.addDescriptor(new BLE2902());

  myService->addCharacteristic(&setValueCharacteristic);
  setValueDescriptor.setValue(SET_VALUE_DESCRIPTOR_TEXT);
  setValueCharacteristic.addDescriptor(&setValueDescriptor);
  setValueCharacteristic.addDescriptor(new BLE2902());

  myServer->getAdvertising()->addServiceUUID(SERVICE_ID);
  myService->start();
  myServer->getAdvertising()->start();
}

void setup() {
  Serial.begin(115200);
  initializeBluetoothLowEnergyService();
}

void loop() {
  getValueCharacteristic.setValue("It worked!");  
  delay(1000);
}
