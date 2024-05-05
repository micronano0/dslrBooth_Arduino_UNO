/* dslrBooth ==> Arduino UNO receive dslrbooth serial commands and switch Yongnuo LED Light
 * dslrbooth_arduino_uno_v1_micronano24.ino
 * Version 1.0 May, 2024
 *
 * receive dslrbooth serial commands and switch Yongnuo LED Light on/off
 * controls the color of a WS2412 LED NeoPixel Ring
 *
 * check the Arduino COM port in the device manager and transfer it to the Windows batch file
 * take notice of readme.h
 * 
 *    
 *
 */
 
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 12                                  // LED RING DIN
#define ANZLED 24                               // Anzahl LEDs im Ring

Adafruit_NeoPixel strip = Adafruit_NeoPixel(ANZLED, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.


int ledaction = 0, countdown = 5, countdown_proz = 0;
long randNumber;
uint16_t rci, rcj;


const int irLEDPin = 3;

using IRSignal = uint16_t[3];

IRSignal ir0 {950, 5000, 950}; // off
IRSignal ir1 {700, 5000, 700}; // on
IRSignal ir11 {250, 5000, 250}; // heller
IRSignal ir10 {500, 5000, 500}; // dunkler

int irSignalLength = 3;
unsigned long phase;

String session_start = "session_start";
String session_end = "session_end";
String countdown_start = "countdown_start";
String count_down_proz = "count_down_proz";
String sharing_screen = "sharing_screen";
String capture_start = "capture_start";


// https://forum.arduino.cc/t/how-to-separate-characters-and-numbers-from-incoming-string-from-serial-port/611086/3
char myData[20] = "";
char myData1[20] = "";
char myData2[2] = "";
int i = 0, j = 0, k = 0, y;


// Countdown Timer
unsigned long countdown_time;
unsigned long startTime;
unsigned long currentTime;
unsigned long elapsedTime;



void setup()
{
  Serial.begin(9600);
  pinMode(irLEDPin, OUTPUT);
  startTime = millis(); // Record the starting time
  strip.begin();
  strip.setBrightness(200);
  strip.show(); // Initialize all pixels to 'off'
}



void loop()
{
  currentTime = millis(); // Get the current time
  elapsedTime = (currentTime - startTime) / 1000; // Calculate elapsed time in seconds

  if (ledaction > 0) {
    if (elapsedTime <= countdown_time) {
      unsigned long remainingTime = countdown_time - elapsedTime;

      if (remainingTime > 0) {
        unsigned long prozentTime = ((100 * remainingTime) / countdown_time);
        //Serial.print("prozentTime = " );
        //Serial.println(prozentTime);  //shows: down
        //if ((prozentTime >= 75) && (prozentTime <= 100)) ledaction = 1;
        if ((prozentTime >= 67) && (prozentTime <= 100)) ledaction = 1;
        if ((prozentTime >= 34) && (prozentTime <= 66)) ledaction = 2;
        if ((prozentTime >= 0) && (prozentTime <= 33)) ledaction = 3;
        //Serial.print("ledaction = " );
        //Serial.println(ledaction);  //shows: down


      }


      if (remainingTime == 0) {
        // Start blinking when countdown reaches 00:00
        IRsenden("off");
        ledaction = 0;
        //countdown_time = 0;
        //startTime = millis(); // Record the starting time
      }
    }

    if (ledaction == 1) {
      colorWipe(strip.Color(0, 255, 0), 50); // Grün
      colorWipe(strip.Color(0, 0, 0), 45); // leer
    }
    if (ledaction == 2) {
      colorBlink(strip.Color(255, 255, 0), 150); // gelb
      colorBlink(strip.Color(0, 0, 0), 150); // leer
    }
    if (ledaction == 3) {
      colorBlink(strip.Color(255, 0, 0), 80); // Rot
      colorBlink(strip.Color(0, 0, 0), 40); // leer
    }
  } else {
    strip.setBrightness(100);
    randNumber = random(256);
    rcj++;
    rci++;

    strip.setPixelColor(rci, Wheel(((rci * 256 / strip.numPixels()) + rcj) & 255));

    strip.show();
    delay(20);
    if (rci > strip.numPixels() ) rci = -1;
    if (rcj > 255 ) rcj = -1;

  } // check ledaction

  byte n = Serial.available();
  if (n != 0)
  {
    byte m = Serial.readBytesUntil('\n', myData, 20);
    for (int i = 0; i < m; i++)
    {
      char x = myData[i];
      if (isdigit(x) == 0)
      {
        myData1[j] = x;  //storing only the charcaters
        j++;
      }
      else
      {
        myData2[k] = x;  //storing only the non-charcaters
        k++;
        //if (k == 2)
        //{
        y = atoi(myData2);  //making the integer part
        //}
      }
    }

    //    strip.setBrightness(200);

    /*    Serial.println(myData1);  //shows: down
        Serial.println(y, DEC);   //shows: 45
        Serial.println("vergleich myData1,'session_start' = " );  //shows: down
        Serial.println(strcmp(myData1,session_start.c_str()));  //shows: down
        Serial.println("vergleich myData1,'session_end' = " );  //shows: down
        Serial.println(strcmp(myData1,session_end.c_str()));  //shows: down
        Serial.println("vergleich myData1,'countdown_start' = " );  //shows: down
        Serial.println(strcmp(myData1,countdown_start.c_str()));  //shows: down
        Serial.println("vergleich myData1,'count_down_proz' = " );  //shows: down
        Serial.println(strcmp(myData1,count_down_proz.c_str()));  //shows: down
        Serial.println("vergleich myData1,'sharing_screen' = " );  //shows: down
        Serial.println(strcmp(myData1,sharing_screen.c_str()));  //shows: down
        Serial.println("vergleich myData1,'capture_start' = " );  //shows: down
        Serial.println(strcmp(myData1,capture_start.c_str()));  //shows: down
    */

    if ((strcmp(myData1, session_start.c_str()) == 0) || (strcmp(myData1, session_start.c_str()) == 13)) {
      IRsenden("on");
      ledaction = 1;
    }
    if ((strcmp(myData1, countdown_start.c_str()) == 0 ) || (strcmp(myData1, countdown_start.c_str()) == 13 ) ) {
      IRsenden("on");
      countdown_time = y, DEC;
      ledaction = 1;
      startTime = millis(); // Record the starting time
    }
    if ((strcmp(myData1, capture_start.c_str()) == 0 ) || (strcmp(myData1, capture_start.c_str()) == 13 )) {
      ledaction = 3;
    }
    if ((strcmp(myData1, sharing_screen.c_str()) == 0 ) || (strcmp(myData1, sharing_screen.c_str()) == 13 )) {
      IRsenden("off");
      ledaction = 0;
    }
    if ((strcmp(myData1, session_end.c_str()) == 0) || (strcmp(myData1, session_end.c_str()) == 13)) {
      IRsenden("off");
      ledaction = 0;
    }


    memset(myData, 0, sizeof myData);  //array reset
    memset(myData1, 0, sizeof myData1); //array reset
    memset(myData2, 0, sizeof myData2); //array reset
    j = 0;
    k = 0;
    y = 0;
  }
} // loop


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void IRsenden(String signal) {
  if (signal == "on") {
    senden(ir1);
    delay(45);
  }

  if (signal == "off") {
    senden(ir0);
    delay(45);
  }
  if (signal == "hell") {
    for (int x = 0; x <= 10; x++) {
      senden(ir11);
      delay(45);
    }
  }
  if (signal == "dunkel") {
    for (int x = 0; x <= 10; x++) {
      senden(ir10);
      delay(45);
    }
  }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------



void senden(int IRSignal[]) {
  int i = 0;
  phase = 20 * IRSignal[i];
  pulseIR(phase);
  i++;

  do {
    phase = 20 * IRSignal[i];
    delayMicroseconds(phase);
    i++;
    phase = 20 * IRSignal[i];
    pulseIR(phase);
    i++;
  }
  while (i < irSignalLength);
}


void pulseIR(long microsecs) {
  cli();  // interrupts switched off

  while (microsecs > 0) {
    // 38 kHz is about 13 microseconds high and 13 microseconds low
    PORTD |= (1 << PD5);              // much faster than
    digitalWrite(irLEDPin, HIGH);   // too slow: 3 microseconds (Arduino UNO / Atmega 328P)
    delayMicroseconds(10);            // 10 microseconds, if you chose digitalWrite
    digitalWrite(irLEDPin, LOW);    // again 3 microseconds
    PORTD &= ~(1 << PD5);
    delayMicroseconds(10);            // 10 microseconds, if you chose digitalWrite
    microsecs -= 26;
  }

  sei();  // interrupts switched on
}





// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

// Fill the dots one after the other with a color
void colorBlink(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();
  delay(wait);
}




// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
