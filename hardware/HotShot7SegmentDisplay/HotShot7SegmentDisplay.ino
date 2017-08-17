
#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>
#include <Seven_Segment_Pixel.h>

// Which pin on the Arduino is connected the NeoPixel display connected to?
#define PIN            6

// How many NeoPixels are in the seven segment display total?
#define NUMPIXELS      128

// How many digits are in the display?
#define DIGITS 6

// How may pixels per segement are there?
#define PIXPERSEG 3

// How many delimeters are in the display?
#define NUMDELIMS 1

// How may pixels per delimeter are there?
#define PIXPERDELIM 2

// When we instatiate a display object we supply the number of digits in the custom built display,
// as well as the number of pixles per segment. Additionally we pass the ususal Adafruit_NeoPixel arguments\
// for object instatiation.

Seven_Segment_Pixel display1 = Seven_Segment_Pixel( DIGITS, PIXPERSEG, NUMDELIMS, PIXPERDELIM, NUMPIXELS, PIN, NEO_RGB + NEO_KHZ800 );


// Stuff for figure8
int figure8[] = {17, 16, 11, 12, 17, 15, 14, 13};
int count = 0;

int REDLEVEL = 255;
int GREENLEVEL = 40;
// Calculate based on max input size expected for one command
#define INPUT_SIZE 40

// Connection to logo board
#define rxPin 8
#define txPin 9
SoftwareSerial softSerial(rxPin, txPin);


void setup() {
  Serial.begin(9600);
  softSerial.begin(9600);

  display1.begin(); // Initialize pins for output
  display1.show();  // Turn all LEDs off ASAP
 
}


unsigned long timer; 
unsigned long timer2; 
unsigned long timer3; 
unsigned long INTERVAL1 = 20;
unsigned long INTERVAL2 = 100;
unsigned long INTERVAL3 = 2;

String command;

String lastscorehuns;
String lastscoretens;
String lastscoreones;
String lastmins;
String lastsectens;
String lastsecs;
String lasteffect;

String scorehuns;
String scoretens;
String scoreones;
String mins;
String sectens;
String secs;
String effect;


void parseCommand(String com) {
  
  effect = com.substring(0, 2);
  scorehuns = com.substring(2, 4);
  scoretens = com.substring(4, 6);
  scoreones = com.substring(6, 8);
  mins = com.substring(8, 10);
  sectens = com.substring(10, 12);
  secs = com.substring(12, 14);

  if (effect.toInt() >= 10 )  {
    if (effect.toInt() == 10 ) {
      off();
    }
 
    } else {

    softSerial.print(effect);
   
     if ( scorehuns != lastscorehuns ) {
      display1.updateDigit( 1, scorehuns.toInt(), REDLEVEL, GREENLEVEL,0);
      scorehuns = lastscorehuns;
    }

    if ( scoretens != lastscoretens ) {
      display1.updateDigit( 2, scoretens.toInt(), REDLEVEL, GREENLEVEL,0);
      scoretens = lastscoretens;
    }
    if ( scoreones != lastscoreones ) {
      display1.updateDigit( 3, scoreones.toInt(), REDLEVEL, GREENLEVEL,0);
      scoreones = lastscoreones;
    }
    if ( mins != lastmins) {
      display1.updateDigit( 4, mins.toInt(), REDLEVEL, GREENLEVEL,0);
      mins = lastmins;
    }
    if ( sectens != lastsectens ) {
      display1.updateDigit( 5, sectens.toInt(), REDLEVEL, GREENLEVEL,0);
      sectens = lastsectens;
    }
    if ( secs != lastsecs ) {
      display1.updateDigit( 6, secs.toInt(), REDLEVEL, GREENLEVEL,0);
      // Flash colon when seconds change
      if  (!(secs.toInt() % 2 )) {
          //  even
          display1.updateDelimiter(1,0,0,0);
      } else {
          //  odd
          display1.updateDelimiter(1,REDLEVEL, GREENLEVEL,0);
        }
      secs = lastsecs;
    }

  }
display1.show();
}


void loop() {
  readserial();

  if ((millis() - timer) > INTERVAL1) {
    timer += INTERVAL1;// reset timer by moving it along to the next interval
    if (effect == "11") {
      // Send random effect code to logo board
      softSerial.print(random(2, 4));
    
      readserial();
       //do the rainbow effect from strandtest
      uint16_t i, j;

      for (j = 0; j < 256; j++) {
        for (i = 0; i <= 125; i++) {
          display1.setPixelColor(i, Wheel((i + j) & 255));
        }
        display1.show();

      }

    }
  }   // end INTERVAL1

  if ((millis() - timer2) > INTERVAL2) {
    timer2 += INTERVAL2;
    if ( effect == "12" ) {
      softSerial.print(2);
      readserial();
      
       // figure8    
      for (int x = 1; x <= DIGITS; x++) {
     display1.updateDigit(x, figure8[count], random(0, 255), random(0, 255),
                         random(0, 255));
  }
  count++;
  display1.show();
  if (count == 8) {
    count = 0;
  }
  display1.show();

    }

    if ( effect == "13" ) {
      softSerial.print(3);
      readserial();
      
     // random segments
      for ( int  v = 1; v <= 6; v++) {
        display1.updateDigit( v, random(11,18),random(0,255), random(0,255), random(255));
      }
      display1.show();
   }
    
  }  // end of INTERVAL2
   
}
  

void readserial() {
  if (Serial.available()) {
   // Serial.println("serail avail");
    char c = Serial.read();

    if (c == '\n') {
      // Once we get a command terminated by a newline, parse it.
      parseCommand(command);
      // Clear out the variable for the next command.
      command = "";
    }
    else {
      // Keep reading, no newline yet.
      command += c;
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return Adafruit_NeoPixel::Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
     return Adafruit_NeoPixel::Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
    WheelPos -= 170;
    return Adafruit_NeoPixel::Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

void off() {
  for (int  a = 0; a <= 127; a++) {
    display1.setPixelColor(a , 0);
    display1.show();
  }
}


