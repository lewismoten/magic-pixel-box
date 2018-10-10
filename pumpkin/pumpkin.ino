#include <PololuLedStrip.h>
#define LED_COUNT 256
#define LED_DATA_PIN 2
PololuLedStrip<LED_DATA_PIN> ledStrip;
rgb_color _ = rgb_color(0, 0, 0);
rgb_color I = rgb_color(0, 127, 14);
rgb_color O = rgb_color(255, 106, 0);
rgb_color x = rgb_color(127, 51, 0);

rgb_color colors[LED_COUNT] = {
  _,_,_,_,_,_,_,_,I,I,_,_,_,_,_,_,
  _,_,_,_,_,_,_,I,I,_,_,_,_,_,_,_,
  _,_,_,_,_,_,_,I,I,_,_,_,_,_,_,_,
  _,_,_,x,O,x,x,I,I,x,x,O,x,_,_,_,
  _,_,x,O,x,O,x,x,O,x,O,x,O,x,_,_,
  _,x,O,O,_,x,O,x,O,O,x,_,O,O,x,_,
  _,O,O,x,_,x,O,x,O,O,x,_,x,O,O,_,
  x,O,O,_,_,_,O,x,O,O,_,_,_,O,O,x,
  x,O,_,_,_,_,O,x,O,O,_,_,_,_,O,x,
  x,O,x,O,O,x,O,x,O,O,x,O,O,x,O,x,
  x,O,x,O,O,x,O,x,O,O,x,O,O,x,O,x,
  x,O,_,O,_,x,_,x,_,_,_,O,_,_,O,x,
  _,O,x,_,_,_,_,_,_,_,_,_,_,x,O,_,
  _,x,O,x,_,O,_,_,_,O,_,_,x,O,x,_,
  _,_,x,O,x,O,x,x,x,x,O,x,O,x,_,_,
  _,_,_,x,O,x,O,_,_,O,x,O,x,_,_,_
};

void setup() {
  reverseOddLines();
  ledStrip.write(colors, LED_COUNT);
}

void loop() {
}

void reverseOddLines() { 
  for (byte y = 1; y < 16; y+=2) {
    int offset = y * 16;
    for(byte x = 0; x < 8; x++) {
      int a = offset + x;
      int b = offset + (15 - x);
      rgb_color temp = colors[a];
      colors[a] = colors[b];
      colors[b] = temp;
    }
  }
}
