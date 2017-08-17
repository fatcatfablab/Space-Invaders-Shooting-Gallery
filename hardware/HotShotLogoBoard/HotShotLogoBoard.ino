#include <Adafruit_DotStar.h>
#include <SoftwareSerial.h>

// Because conditional #includes don't work w/Arduino sketches...
#include <SPI.h>         // COMMENT OUT THIS LINE FOR GEMMA OR TRINKET
//#include <avr/power.h> // ENABLE THIS LINE FOR GEMMA OR TRINKET
#include <math.h>

#define NUMPIXELS 153 // Number of LEDs in strip

// Here's how to control the LEDs from any two pins:
#define DATAPIN    4
#define CLOCKPIN   5
Adafruit_DotStar strip = Adafruit_DotStar(
                           NUMPIXELS, DATAPIN, CLOCKPIN);


#define O 17
#define P 9

// Artisinal handcrafted asymetric matrix

int ledMatrix[O][P] = {
 
  {  0,   30,   31,   63,  64,  97,  98,  130,  131  },
 
  {  1,   29,  32,   62,  65,  96,   99,  129,  132   },
 
  {  2,   28,  33,   61,  66,  95,  100,  128,  133   },
  
  {  3,   27,  34,   60,  67,  94,  102,  127,  134   },
 
  {  4,   26,   35,  59,  68,  93,  103,  126,  135  },
  
  {  5,   25,   36,  58,  69,  92,  104,  125,  136  },
  
  {  6,   24,  37,  57,   70,  91,  105,  124,  137 },
  
  {  7,   23,  38,  56,   71,  90,  106,  123,  138  },
 
  {  8,   22,  39,  55,   72,  89,  107,  122,  139  },
  
  {  9,   21,  40,  54,   73,  88,  108,  121,  140  },
  
  {  10,   20,  41,  53,  74,  87, 109,  120,  141 },
  
  {  11,   19,  42,  52,  75,  86, 110,  119, 142 },
  
  {  12,   18,  43,  51,  76,  85, 111,  118, 143  },
  
  {  13,   17,  44,  50,  77,  84, 112, 117, 144 },
  
  {  14,  16,   45,  49,  78,  83, 113, 116, 145 },
  
  {  152,  15,  46,  48,  79,  82, 114, 115, 152},

  {  152,  152,  152,  47,  80,  81, 152, 152, 152  },                          
        
 
};

int j = 0;
int x;
int y;
int command= 0;

#define rxPin 11
#define txPin 10
SoftwareSerial softSerial(rxPin, txPin);
const byte interruptPin = 2;

char  previousByte;

void setup() {

  Serial.begin(9600);
      softSerial.begin(9600);
  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP
  strip.setBrightness(255);

  delay(3000);
  
  command = 3; 
  flash(5);
  int g = 0;
 
  Serial.println(ledMatrix[0][1]);

 }



void loop() {
  readserial();

   if  (command == 0) {
    off();
  }
   if  (command == 1) {
    sleep();
  }
  if  (command == 2) {
    noise();
  }
  if  (command == 3) {
    flash(11);
  }
  if  (command == 4) {
    konami(1);
  }
   if  (command == 5) {
    diagTL();
  }
 }


void wipe( int b, int t, char* d ) {
 
  if (d == "u") {
    for (  x = 16; x >= 0; x--) {
      for (  y = 8; y >=0; y--) {
        strip.setPixelColor(ledMatrix[x][y], b);

             }
      strip.show();
      delay(t);
    }
  }

  if (d == "d") {
    for (  x = 0; x <=16; x++) {
      for (  y = 0; y <= 8; y++) {
        strip.setPixelColor(ledMatrix[x][y], b);
        }
      strip.show();
     delay(t);
    }
  }

  if (d == "r") {
       for (  y = 0; y <= 8; y++) {
       for( x = 0; x <= 16; x++) {
      strip.setPixelColor(ledMatrix[x][y], b);
      
       }
      strip.show();
        delay(t);
    }
  }


  if (d == "l") {
    for (  y = 8; y >= 0; y--) {
        for (  x = 0; x <= 16; x++) {
      strip.setPixelColor(ledMatrix[x][y], b);

      
         }
      strip.show();
      delay(t);
    }
  }


}


void noise() {
  while ( command == 2) {
    readserial();
    for ( x = 0; x <= 150; x++) {
      int rawval = analogRead(A0);
      int b = map (rawval, 0, 5, 0, 255);
      strip.setPixelColor(x, b);
   }
    strip.show();
    
  }
}

void flash( int t) {
   Serial.println("this is flash");
  int h = 0;
  while ( h <= t-1) {
    
    for ( x = 0; x <= 149 ; x++) {
      strip.setPixelColor(x, 255);
    }
    strip.show();
    delay(100);
    for ( x = 0; x <= 149 ; x++) {
      strip.setPixelColor(x, 0);

    }
    strip.show();
    delay(100);
    h++;

  }
  command =0;

}


void sleep() {
  while ( command == 1) {
    readserial();
    float b = (exp(sin(millis() / 2000.0 * PI)) - 0.36787944) * 90.0;
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, b);
    }
    strip.show();
  }
}


void konami ( int t) {
 if (command == 4 ) { 
  
 wipe(255 ,t, "u");
       wipe(0, t, "u");
       wipe(255 ,t, "u");
      wipe(0, t, "u");

 wipe(255 ,t, "d");
     wipe(0, t, "d");
       wipe(255 ,t, "d");
      wipe(0, t, "f");
     
     

   wipe(255 , t,"l");
     wipe(0,t, "l");


   wipe(255 , t,"r");
     wipe(0,t, "r");

  
   
   wipe(255 , t,"l");
     wipe(0,t, "l");


   wipe(255 , t,"r");
     wipe(0,t, "r");
 }
}

void diagTL() {


        int i, j, t;
         for ( t = 0; t<O+P; ++t) {
              for( i=t, j=0; i>=0 ; --i, ++j) {
                     if( (i<O) && (j<P) ) { 
                       strip.setPixelColor(ledMatrix[i][j], 255);
                     }
                           
                            
                     } 
             strip.show();  
             delay(2);    
}
for ( t = 0; t<O+P; ++t) {
              for( i=t, j=0; i>=0 ; --i, ++j) {
                     if( (i<O) && (j<P) ) { 
                      strip.setPixelColor(ledMatrix[i][j], 0);
                     }
                           
                            
                     } 
             strip.show();  
             delay(2);    
}

}

void wipes( int t ) {

wipe(255 ,t, "d");
     wipe(0, t, "d");
  
}

void readserial() {
if (softSerial.available()) {
  char  incomingByte = (softSerial.read());
    
    if ( previousByte != incomingByte ) {
    Serial.println(incomingByte);
    previousByte = incomingByte;
    }
    
    //sanitize input
    if (incomingByte == '0') {   
     command =0;
      }    
      
     if (incomingByte == '1') {   
     command = 1;
     
      }      
 if (incomingByte == '2') {   
    command = 2;
      }      
 if (incomingByte == '3') {   
    command = 3;
      }      

 if (incomingByte == '4') {   
      command = 4;
      }      
  }
}

void off() { 
  for ( int u =0; u <= 152;u++) {
    strip.setPixelColor(u, 0);
  }
  strip.show();
  
}

