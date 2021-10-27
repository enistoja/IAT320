
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

//pin being used

#define LED_PIN 6

//number of NeoPixels attached
#define LED_COUNT 2

//declare NeoPixel strip object, including bitstream of 800KHz and colour as GRB
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

const int buttonPin = 10; //input digital pin
const int ledPin = 6;
const int buzzPin = 9;
int buttonState = 0; //button status
int brightness = 255;


void setup() {
  //just in case we are using adafruit trinket 6v 16 MHz
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif
  
  pinMode(buttonPin, INPUT_PULLUP); //look up what internal pullup resistors are

  pinMode(buzzPin, OUTPUT);
  
  Serial.begin(9600);

  //initialize light strip
  strip.begin();
  strip.show();

  strip.setBrightness(brightness);
  strip.setPixelColor(0, strip.gamma32(strip.ColorHSV(0,0,100)));
  strip.setPixelColor(1, strip.gamma32(strip.ColorHSV(0,0,100)));
}

void loop() {
  //check button value
  bool buttonState = digitalRead(buttonPin);


  if (!buttonState)
  {
    //set colour to white, update strip
    strip.setPixelColor(0, strip.gamma32(strip.ColorHSV(0,0,100)));
    strip.setPixelColor(1, strip.gamma32(strip.ColorHSV(0,0,100)));
    strip.show();

    //turn speaker on
    digitalWrite(buzzPin, HIGH);

    delay(200);
  }
  else
  {
    //set colour to black, update strip
    strip.setPixelColor(0, strip.gamma32(strip.ColorHSV(0,0,4)));
    strip.setPixelColor(1, strip.gamma32(strip.ColorHSV(0,0,4)));
    strip.show();

    //turn speaker off
    digitalWrite(buzzPin, LOW);
    
    delay(200);
  }

  

}
