//code adapted from strandtest for Adafruit_NeoPixel

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


//analog declarations
#define analogInPin  A9

int sensorValue = 0; //input

//global variables

int brightness = 40;
int switchSpeed = 100;

int pressureBrightness = 0;

int firstPixelHue = 0;

void setup() {
  //just in case we are using adafruit trinket 6v 16 MHz
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif

//initialize serial comms at 9600 bps
  Serial.begin(9600);

  strip.begin(); //initialize object
  strip.show(); //turn off ALL pixels
  strip.setBrightness(brightness); //set brightness to 20/255 brightness, too bright otherwise!
}

void loop() {

  //read sensor
  sensorValue = analogRead(analogInPin);

  //map value to brightness;
  pressureBrightness = map(sensorValue, 0, 1023, 20, 0);


  if (firstPixelHue < 5*65536)
  {
    firstPixelHue += 1024;
    for(int i=0; i<strip.numPixels(); i++) { //do this for each pixel in strip
      //offset hue to give each wheel one full revolution (65536)
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());

      //assign pixelHue to pixel
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    //set brightness based on pressure sensor
    strip.setBrightness(pressureBrightness);
    strip.show();
  }
  else
  {
    firstPixelHue = 0;
  }
  delay(200);
 
}
