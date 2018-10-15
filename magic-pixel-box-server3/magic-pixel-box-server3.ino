#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <FastLED.h>

#define NUM_LEDS 256
#define LED_DATA_PIN 2
CRGB leds[NUM_LEDS];

// If any bluetooth settings change you may need to forget and pair the device to clear cached settings
// ie - id's, services, descriptors, characteristics, etc.

#define DEVICE_NAME "Magic Pixel Box"
#define SERVICE_ID "4c657769-7320-4d6f-7465-6e0000000004"
#define GET_COLOR_ID "4c657769-7320-4d6f-7465-6e0000000005"
#define SET_COLOR_ID "4c657769-7320-4d6f-7465-6e0000000006"

BLECharacteristic getColor(GET_COLOR_ID, BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_INDICATE);
BLECharacteristic setColor(SET_COLOR_ID, BLECharacteristic::PROPERTY_WRITE);

typedef struct {
  byte R;
  byte G;
  byte B;
} Pixel;

Pixel pixels[256];

bool _connected = false;

int getPixelIndex(byte x, byte y) {
  if (y % 2 == 0) {
    x = 17 - x;
  }
  return ((y - 1) * 16) + (x - 1);
}

class ServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* server) {
      _connected = true;
    };
    void onDisconnect(BLEServer* server) {
      _connected = false;
    }
};

class GetColorCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *characteristic) {

     std::string value = getColor.getValue();
     byte x = value[0];
     byte y = value[1];

      int index = getPixelIndex(x, y);
      Pixel pixel = pixels[index];

      byte values[5] = {
        x,
        y,
        pixel.R,
        pixel.G,
        pixel.B
      };

      getColor.setValue({x, y, pixel.R, pixel.G, pixel.B});
      getColor.indicate();
      // NOTE: without a delay, bluetooth seems to hang
      delay(10);
  }
};

class SetColorCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *characteristic) {
    byte x = setColor.getValue()[0];
    byte y = setColor.getValue()[1];
    byte r = setColor.getValue()[2];
    byte g = setColor.getValue()[3];
    byte b = setColor.getValue()[4];
    int index = getPixelIndex(x, y);
    
    pixels[index].R = r;
    pixels[index].G = g;
    pixels[index].B = b;

    leds[index] = CRGB(r, g, b);
    FastLED.show();
  }
};

void initializeBluetoothLowEnergyService() {
  BLEDevice::init(DEVICE_NAME);
  BLEServer *myServer = BLEDevice::createServer();
  myServer->setCallbacks(new ServerCallbacks());
  BLEService *myService = myServer->createService(SERVICE_ID);

  myService->addCharacteristic(&getColor);
  getColor.setCallbacks(new GetColorCallbacks());
  myService->addCharacteristic(&setColor);
  setColor.setCallbacks(new SetColorCallbacks());

  getColor.addDescriptor(new BLE2902());
  setColor.addDescriptor(new BLE2902());

  myServer->getAdvertising()->addServiceUUID(SERVICE_ID);
  myService->start();
  myServer->getAdvertising()->start();
}

void setup() {
  Serial.begin(115200);
  FastLED.addLeds<NEOPIXEL, LED_DATA_PIN>(leds, NUM_LEDS);
  initializeBluetoothLowEnergyService();
  FastLED.show();
}
void loop() {
}
