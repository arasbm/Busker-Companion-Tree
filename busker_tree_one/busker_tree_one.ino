// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      150

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
const int minDelay = 3;
const int maxDelay = 50;
int delayVal = 50;

const int sensor = A0; // the piezo is connected to analog pin 0
int sensorVal = 0;     // variable to store the value read from the sensor pin
int halfDist = 0;
int thirdDist = 0;
int pos = 0; // start position of animation

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  pixels.begin(); // This initializes the NeoPixel library.
  //Serial.begin(9600);
}

void loop() {
  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
  pos = (pos + 1) % NUMPIXELS;
  sensorVal = analogRead(sensor) + 2;
  halfDist = (sensorVal*sensorVal/2) % 40;
  thirdDist = (sensorVal*sensorVal/3) % 20;
  for(int i=0;i<NUMPIXELS;i++){
    if(i < pos - halfDist || i > pos + halfDist) {
      pixels.setPixelColor(i, pixels.Color(0,0,0));
    } else if(i < pos - thirdDist || i > pos + thirdDist) {
      pixels.setPixelColor(i, Wheel(sensorVal % 255));
    } else if(i == pos) {
      pixels.setPixelColor(i, pixels.Color(255,0,0));
    } else {
      pixels.setPixelColor(i, Wheel(random(255)));
    }
  }
  // throw in some sparkles
  for(int j=0;j<(thirdDist % 50) / 3;j++) {
    pixels.setPixelColor(random(149), Wheel(random(255)));
  }

  // slow down after a while when nothing is happening
  if(halfDist < 10) {
    if(delayVal < maxDelay && (pos % 20) == 0) {
      delayVal += 1;
    }
  }
  
  if(halfDist > 9) {
    // speed up when action is happening
    if(delayVal > minDelay) {
      delayVal = delayVal - 5;
      if(delayVal < minDelay) {
        delayVal = minDelay;
      }
    }
  }

  pixels.show(); // This sends the updated pixel color to the hardware
  delay(delayVal);
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
