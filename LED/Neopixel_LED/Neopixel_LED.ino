#include <Adafruit_NeoPixel.h>

#include <Adafruit_NeoPixel.h>
#define PIN 6
#define NUMPIXELS 12

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 500;

void setup() {
  pixels.begin();
}

void loop() {
  for(int i =0; i <NUMPIXELS; i++){
    pixels.setPixelColor(i,pixels.Color(0,150,0));
    pixels.show();
    delay(delayval);
  }
}
