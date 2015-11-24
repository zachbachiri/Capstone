// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            9

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      60

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 50; // delay for half a second
const int trigPin = 7;
const int echoPin = 8;
int ledPin = 9;
int lastBrightness = 1;
int brightChange = 1;

void setup() {
  pixels.begin(); // This initializes the NeoPixel library.
  Serial.begin(9600);
}

void loop() {
  double duration, inches, cm, ft;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);

  // convert the time into a distance
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);
  ft = inches/12;
  //72 -> 6 feet, a distance where we think light should start increasing brightness
  double brightnessDouble;
  Serial.print("ft");
  Serial.print(ft);
  Serial.println();
  
  if (ft < 5.5) {
    brightnessDouble = 0.00300595*pow(ft,8)-
                        0.110298*pow(ft,7)+
                        1.71389*pow(ft,6)-
                        14.7021*pow(ft,5)+
                        76.0674*pow(ft,4)-
                        243.307*pow(ft,3)+
                        472.416*pow(ft,2)-
                        517.081*ft+
                        255;
  } else if(ft > 100) {
    brightnessDouble = lastBrightness;
  } else {
    brightnessDouble = 1;
  }
  
  int brightness = (int) brightnessDouble;
  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.

  //Serial.print(brightness);
  //Serial.println();
  bool increase = brightness > lastBrightness;
  if (abs(brightness-lastBrightness)>brightChange){
    if(increase) {
      brightness=min(lastBrightness+brightChange,255);
    } else {
      brightness=max(lastBrightness-brightChange,5);
    } 
  }

  //Serial.print(brightness);
  //Serial.println();
  setBrightness(brightness);
  lastBrightness = brightness;
  
  //Serial.print(brightness);
  //Serial.println();

  
  
}

void setBrightness(int brightness) {
    
      for(int i=0;i<NUMPIXELS;i++){
        pixels.setPixelColor(i, pixels.Color(brightness,brightness,brightness)); // Moderately bright red color.
        
        //Serial.print(currentBrightness);
        //Serial.print("+");
        //Serial.println();
      }
      pixels.show();
}

long microsecondsToInches(long microseconds) {
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}
