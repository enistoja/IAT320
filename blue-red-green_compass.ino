#include <Adafruit_LSM303DLH_Mag.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
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

//declare mag sensor
Adafruit_LSM303DLH_Mag_Unified mag = Adafruit_LSM303DLH_Mag_Unified(12345);

//compass angle
int angle = 0;
int tempangle = 0;
int red = 0;
int green = 0;
int blue = 0;


void setup(void) {
  //just in case we are using adafruit trinket 6v 16 MHz
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif
  
  Serial.begin(115200);
  //Serial.begin(9600);
  /* Initialise the sensor */
  if (!mag.begin()) {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("No sensor detected");
    while (1)
      ;
  }
  Serial.println("Sensor sensed");

  strip.begin(); //initialize object
  strip.show(); //turn off ALL pixels
  strip.setBrightness(200); //brightness out of max of 255;

}

void loop(void) {
  /* Get a new sensor event */
  sensors_event_t event;
  mag.getEvent(&event);

  float Pi = 3.14159;

  // Calculate the angle of the vector y,x
  float heading = (atan2(event.magnetic.y, event.magnetic.x) * 180) / Pi;
  Serial.print("angle: ");
  Serial.println(heading);
  
  // Normalize to 0-360
  if (heading < 0) {
    heading = 360 + heading;
  }

  //set red. if at 0 degrees it is full red, if it is 180 red is 0
  tempangle = heading;
  //if the angle is greater than 180, see what the equivalent angle is
  if (tempangle > 180)
  {
    tempangle = 180 - (tempangle-180);
  }
  red = map (tempangle, 0, 180, 0, 255);
  
  //set blue. Blue is the opposite of red, so simply map it backwards
  blue = map (red, 0, 255, 255, 0);

  //set green. Green points West, so we rotate the math for Red 90 degrees. This way East is purple.
  tempangle+=90;
  tempangle = map (tempangle, 90, 450, 0, 360);
  if (tempangle > 180)
  {
    tempangle = 180 - (tempangle-180);
  }
  green = map(tempangle, 0, 180, 0, 255);

  //set strip colours.
  strip.setPixelColor(0, strip.Color(red, green, blue));
  strip.setPixelColor(1, strip.Color(red, green, blue));

  strip.show();
  
  
  delay(100);
}
