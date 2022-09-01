// LED Strip Controller

#include <FastLED.h>
#include <limits.h>
#include <math.h>

#include "LEDController.h"
#include "DimmerModes.h"
#include "LEDPackets.h"

#ifndef __FPU_USED
#define __FPU_USED
#endif /* __FPU_USED */


CRGB aleds[ALED_COUNT];

CRGBSet aledsSet(aleds, ALED_COUNT);

const uint16_t s[6][2] = {{0, 82},
                          {328, 409},
                          {167, 248},
                          {82, 166}, // reverse direction for last 3 strips, also note 166 not 167
                          {409, 490},
                          {248, 327}};

// Other Globals
unsigned long microsPrevious;
unsigned int rollOver;
const uint16_t serialBufferSize = 256;
char serialBuffer[serialBufferSize], serialPacket[serialBufferSize];
uint8_t serialPacketState, serialBytesExpected, serialPacketType, serialPacketIndex;
unsigned int updateMicros, updateMicrosSinceLastReading;
unsigned long microsNow;
unsigned long microsThisReading;

uint8_t currentColorMode = 0;
uint8_t currentDimmerMode = 0;
uint8_t currentAMode = 0;

GUARTmsgDMXUpdate dmx;

void setup( ) {
  // Debug serial port enable
  Serial.begin(57600);

  // Onboard LED power indicator
  pinMode(POWER_ON_LED_PIN, OUTPUT);
  digitalWrite(POWER_ON_LED_PIN, HIGH);

  // Setup LEDs
  FastLED.addLeds<NEOPIXEL, ALED_PIN>(aleds, ALED_COUNT);
  //LEDS.addLeds<WS2812, ALED_PIN, RGB>(aleds, ALED_COUNT);
  //FastLED.addLeds<DOTSTAR, ALED_PIN, 13, GBR>(aleds, ALED_COUNT);

  //FastLED.setBrightness(64); // 64 should be 5A
  // DEPLOY SETTINGS
  //set_max_power_in_volts_and_milliamps(5, 10000);

  // DEBUG SETTINGS
  set_max_power_in_volts_and_milliamps(5, 10000);
  // NOTES: 5A dc fuse popped at 8A
  //        7.5A dc fuse popped at 12A, 10A seems to be a good setting
  
  //for(int i = 0; i < ALED_COUNT; i++) aleds[i] = CRGB::Black;
  FastLED.clear( );
  FastLED.show( );

  // XBee wireless serial port enable
  Serial1.begin(19200);
  Serial1.setTimeout(0);
  serialPacketState = 0;

  // Timing engine
  updateMicros = 1000000 / 1000;
  updateMicrosSinceLastReading = 0;
  rollOver = 0;
  microsPrevious = micros( );

  Serial.println("Finished setup( )");
}

void loop( ) {
  microsNow = micros( );
  if(microsNow < microsPrevious) {
    rollOver++;
    microsThisReading = microsNow + (UINT_MAX - microsPrevious);
  }
  else {
    microsThisReading = microsNow - microsPrevious;
  }

  static unsigned long microsSec = micros( ) + 1000000;
  static int count = 0;
  if(microsNow >= microsSec) {
    Serial.print("Loops per second: ");
    Serial.print(count);
    Serial.print(" micros: ");
    Serial.print(microsNow);
    Serial.print(" microsThisReading: ");
    Serial.println(microsThisReading);
    microsSec += 1000000;
    count = 0;
  }
  else count++;


  updateMicrosSinceLastReading += microsThisReading;

  if(1) {
  //if(updateMicrosSinceLastReading >= updateMicros) {
    // Things we only send at the frequency specified in #define (prob 100 Hz)

    serialAssemblePacket( );

//    Serial.print(dmx.masterDimmer); Serial.print(" "); Serial.print(dmx.dimmerMode); Serial.print(" ");
//    Serial.print(dmx.d1); Serial.print(" ");
//    Serial.print(dmx.r1); Serial.print(" "); Serial.print(dmx.g1); Serial.print(" "); Serial.print(dmx.b1); Serial.print(" ");
//    Serial.print(dmx.d2); Serial.print(" ");
//    Serial.print(dmx.r2); Serial.print(" "); Serial.print(dmx.g2); Serial.print(" "); Serial.print(dmx.b2); Serial.print(" ");
//    Serial.print(dmx.d3); Serial.print(" ");
//    Serial.print(dmx.r3); Serial.print(" "); Serial.print(dmx.g3); Serial.print(" "); Serial.print(dmx.b3); Serial.print(" ");
//    Serial.print(dmx.d4); Serial.print(" ");
//    Serial.print(dmx.r4); Serial.print(" "); Serial.print(dmx.g4); Serial.print(" "); Serial.print(dmx.b4); Serial.print(" ");
//    Serial.print(dmx.d5); Serial.print(" ");
//    Serial.print(dmx.r5); Serial.print(" "); Serial.print(dmx.g5); Serial.print(" "); Serial.print(dmx.b5); Serial.print(" ");
//    Serial.print(dmx.d6); Serial.print(" ");
//    Serial.print(dmx.r6); Serial.print(" "); Serial.print(dmx.g6); Serial.print(" "); Serial.print(dmx.b6); Serial.print(" ");
//    Serial.print(dmx.colorMode); Serial.println( );
//

    /// COLOR MODE EFFECTS ---------------------------------------
    
    if(dmx.colorMode <= 9) {
      // Right to Left
      aledsSet(s[0][0], s[0][1]) = CRGB(dmx.r1, dmx.g1, dmx.b1);
      aledsSet(s[1][0], s[1][1]) = CRGB(dmx.r2, dmx.g2, dmx.b2);
      aledsSet(s[2][0], s[2][1]) = CRGB(dmx.r3, dmx.g3, dmx.b3);
      aledsSet(s[3][0], s[3][1]) = CRGB(dmx.r4, dmx.g4, dmx.b4);
      aledsSet(s[4][0], s[4][1]) = CRGB(dmx.r5, dmx.g5, dmx.b5);
      aledsSet(s[5][0], s[5][1]) = CRGB(dmx.r6, dmx.g6, dmx.b6);
    }
    else if((dmx.colorMode >= 10) && (dmx.colorMode <= 19)) {
      // Up to Down
      CRGB c = CRGB(dmx.r1, dmx.g1, dmx.b1);
      aledsSet(s[0][0], s[0][0] + 14) = c;
      aledsSet(s[1][0], s[1][0] + 14) = c;
      aledsSet(s[2][0], s[2][0] + 14) = c;
      aledsSet(s[3][1], s[3][1] - 14) = c;
      aledsSet(s[4][1], s[4][1] - 14) = c;
      aledsSet(s[5][1], s[5][1] - 14) = c;

      c = CRGB(dmx.r2, dmx.g2, dmx.b2);
      aledsSet(s[0][0] + 14, s[0][0] + 28) = c;
      aledsSet(s[1][0] + 14, s[1][0] + 28) = c;
      aledsSet(s[2][0] + 14, s[2][0] + 28) = c;
      aledsSet(s[3][1] - 14, s[3][1] - 28) = c;
      aledsSet(s[4][1] - 14, s[4][1] - 28) = c;
      aledsSet(s[5][1] - 14, s[5][1] - 28) = c;

      c = CRGB(dmx.r3, dmx.g3, dmx.b3);
      aledsSet(s[0][0] + 28, s[0][0] + 42) = c;
      aledsSet(s[1][0] + 28, s[1][0] + 42) = c;
      aledsSet(s[2][0] + 28, s[2][0] + 42) = c;
      aledsSet(s[3][1] - 28, s[3][1] - 42) = c;
      aledsSet(s[4][1] - 28, s[4][1] - 42) = c;
      aledsSet(s[5][1] - 28, s[5][1] - 42) = c;

      c = CRGB(dmx.r4, dmx.g4, dmx.b4);
      aledsSet(s[0][0] + 42, s[0][0] + 56) = c;
      aledsSet(s[1][0] + 42, s[1][0] + 56) = c;
      aledsSet(s[2][0] + 42, s[2][0] + 56) = c;
      aledsSet(s[3][1] - 42, s[3][1] - 56) = c;
      aledsSet(s[4][1] - 42, s[4][1] - 56) = c;
      aledsSet(s[5][1] - 42, s[5][1] - 56) = c;

      c = CRGB(dmx.r5, dmx.g5, dmx.b5);
      aledsSet(s[0][0] + 56, s[0][0] + 70) = c;
      aledsSet(s[1][0] + 56, s[1][0] + 70) = c;
      aledsSet(s[2][0] + 56, s[2][0] + 70) = c;
      aledsSet(s[3][1] - 56, s[3][1] - 70) = c;
      aledsSet(s[4][1] - 56, s[4][1] - 70) = c;
      aledsSet(s[5][1] - 56, s[5][1] - 70) = c;

      c = CRGB(dmx.r6, dmx.g6, dmx.b6);
      aledsSet(s[0][0] + 70, s[0][1]) = c;
      aledsSet(s[1][0] + 70, s[1][1]) = c;
      aledsSet(s[2][0] + 70, s[2][1]) = c;
      aledsSet(s[3][1] - 70, s[3][0]) = c;
      aledsSet(s[4][1] - 70, s[4][0]) = c;
      aledsSet(s[5][1] - 70, s[5][0]) = c;
    }
    else if((dmx.colorMode >=20) && (dmx.colorMode <= 29)) {
      static uint8_t hue = 0;
      // First slide the led in one direction
      for(int i = 0; i < ALED_COUNT; i+=1) {
        aleds[i] = CHSV(hue++, 255, 255);
        // Show the leds
        //FastLED.show( );
        //delay(10);
        // now that we've shown the leds, reset the i'th led to black
        // leds[i] = CRGB::Black;
      }
    }
    // two modes, one we blend colors so they are uniform to each strip.
    // the other we blend them so that they are spread through all 6 strips.
    else if((dmx.colorMode >= 30) && (dmx.colorMode <= 39)) {
      float ratio1 = (dmx.r2 - dmx.r1) / (float)(14 + 7);
      float ratio2 = (dmx.g2 - dmx.g1) / (float)(14 + 7);
      float ratio3 = (dmx.b2 - dmx.b1) / (float)(14 + 7);

//      uint32_t ratio1 = ((dmx.r2 - dmx.r1) * 1000000) / (14 + 7);
//      uint32_t ratio2 = ((dmx.g2 - dmx.g1) * 1000000) / (14 + 7);
//      uint32_t ratio3 = ((dmx.b2 - dmx.b1) * 1000000) / (14 + 7);

      CRGB f;

      for(int j = 0; j < (14 + 7); j++) {
//        f = CRGB(scale8(((dmx.r1 * 1000000) + (ratio1 * j)) / 1000000, 255), 
//                 scale8(((dmx.g1 * 1000000) + (ratio2 * j)) / 1000000, 255),
//                 scale8(((dmx.b1 * 1000000) + (ratio3 * j)) / 1000000, 255));
        f = CRGB(dmx.r1 + (ratio1 * j), dmx.g1 + (ratio2 * j), dmx.b1 + (ratio3 * j));
        aleds[s[0][0] + j] = f;
        aleds[s[1][0] + j] = f;
        aleds[s[2][0] + j] = f;
        aleds[s[3][1] - j] = f;
        aleds[s[4][1] - j] = f;
        aleds[s[5][1] - j] = f;
      }

      ratio1 = (dmx.r3 - dmx.r2) / 14.0;
      ratio2 = (dmx.g3 - dmx.g2) / 14.0;
      ratio3 = (dmx.b3 - dmx.b2) / 14.0;
      for(int j = 0; j < 14; j++) {
        f = CRGB(dmx.r2 + (ratio1 * j), dmx.g2 + (ratio2 * j), dmx.b2 + (ratio3 * j));
        aleds[s[0][0] + (14 + 7) + j] = f;
        aleds[s[1][0] + (14 + 7) + j] = f;
        aleds[s[2][0] + (14 + 7) + j] = f;
        aleds[s[3][1] - (14 + 7) - j] = f;
        aleds[s[4][1] - (14 + 7) - j] = f;
        aleds[s[5][1] - (14 + 7) - j] = f;
      }

      ratio1 = (dmx.r4 - dmx.r3) / 14.0;
      ratio2 = (dmx.g4 - dmx.g3) / 14.0;
      ratio3 = (dmx.b4 - dmx.b3) / 14.0;
      for(int j = 0; j < 14; j++) {
        f = CRGB(dmx.r3 + (ratio1 * j), dmx.g3 + (ratio2 * j), dmx.b3 + (ratio3 * j));
        aleds[s[0][0] + (28 + 7) + j] = f;
        aleds[s[1][0] + (28 + 7) + j] = f;
        aleds[s[2][0] + (28 + 7) + j] = f;
        aleds[s[3][1] - (28 + 7) - j] = f;
        aleds[s[4][1] - (28 + 7) - j] = f;
        aleds[s[5][1] - (28 + 7) - j] = f;
      }

      ratio1 = (dmx.r5 - dmx.r4) / 14.0;
      ratio2 = (dmx.g5 - dmx.g4) / 14.0;
      ratio3 = (dmx.b5 - dmx.b4) / 14.0;
      for(int j = 0; j < 14; j++) {
        f = CRGB(dmx.r4 + (ratio1 * j), dmx.g4 + (ratio2 * j), dmx.b4 + (ratio3 * j));
        aleds[s[0][0] + (42 + 7) + j] = f;
        aleds[s[1][0] + (42 + 7) + j] = f;
        aleds[s[2][0] + (42 + 7) + j] = f;
        aleds[s[3][1] - (42 + 7) - j] = f;
        aleds[s[4][1] - (42 + 7) - j] = f;
        aleds[s[5][1] - (42 + 7) - j] = f;
      }

      ratio1 = (dmx.r6 - dmx.r5) / 20.0;
      ratio2 = (dmx.g6 - dmx.g5) / 20.0;
      ratio3 = (dmx.b6 - dmx.b5) / 20.0;
      for(int j = 0; j < (s[0][1] - (56 + 7)); j++) 
        aleds[s[0][0] + (56 + 7) +  j] = CRGB(dmx.r5 + (ratio1 * j), dmx.g5 + (ratio2 * j), dmx.b5 + (ratio3 * j));
      for(int j = 0; j < (s[1][1] - (56 + 7) - s[1][0]); j++) 
        aleds[s[1][0] + (56 + 7) +  j] = CRGB(dmx.r5 + (ratio1 * j), dmx.g5 + (ratio2 * j), dmx.b5 + (ratio3 * j));
      for(int j = 0; j < (s[2][1] - (56 + 7) - s[2][0]); j++) 
        aleds[s[2][0] + (56 + 7) +  j] = CRGB(dmx.r5 + (ratio1 * j), dmx.g5 + (ratio2 * j), dmx.b5 + (ratio3 * j));
        
      for(int j = 0; j < (s[3][1] - (56 + 7) - s[3][0]); j++) 
        aleds[s[3][1] - (56 + 7) -  j] = CRGB(dmx.r5 + (ratio1 * j), dmx.g5 + (ratio2 * j), dmx.b5 + (ratio3 * j));
      for(int j = 0; j < (s[4][1] - (56 + 7) - s[4][0]); j++) 
        aleds[s[4][1] - (56 + 7) -  j] = CRGB(dmx.r5 + (ratio1 * j), dmx.g5 + (ratio2 * j), dmx.b5 + (ratio3 * j));
      for(int j = 0; j < (s[5][1] - (56 + 7) - s[5][0]); j++) 
        aleds[s[5][1] - (56 + 7) -  j] = CRGB(dmx.r5 + (ratio1 * j), dmx.g5 + (ratio2 * j), dmx.b5 + (ratio3 * j));
    }
    else if((dmx.colorMode >= 40) && (dmx.colorMode <= 49)) {
      // blend colors throughout the whole strip
      float ratio1 = (dmx.r2 - dmx.r1) / (float)(s[0][1] - s[0][0]);
      float ratio2 = (dmx.g2 - dmx.g1) / (float)(s[0][1] - s[0][0]);
      float ratio3 = (dmx.b2 - dmx.b1) / (float)(s[0][1] - s[0][0]);
      for(int j = 0; j < (s[0][1] - s[0][0]); j++)
        aleds[s[0][0] + j] = CRGB(dmx.r1 + (ratio1 * j), dmx.g1 + (ratio2 * j), dmx.b1 + (ratio3 * j));
      
      ratio1 = (dmx.r3 - dmx.r2) / (float)(s[1][1] - s[1][0]);
      ratio2 = (dmx.g3 - dmx.g2) / (float)(s[1][1] - s[1][0]);
      ratio3 = (dmx.b3 - dmx.b2) / (float)(s[1][1] - s[1][0]);
      for(int j = 0; j < (s[1][1] - s[1][0]); j++)
        aleds[s[1][1] - j - 1] = CRGB(dmx.r2 + (ratio1 * j), dmx.g2 + (ratio2 * j), dmx.b2 + (ratio3 * j));
        
      ratio1 = (dmx.r4 - dmx.r3) / (float)(s[2][1] - s[2][0]);
      ratio2 = (dmx.g4 - dmx.g3) / (float)(s[2][1] - s[2][0]);
      ratio3 = (dmx.b4 - dmx.b3) / (float)(s[2][1] - s[2][0]);
      for(int j = 0; j < (s[2][1] - s[2][0]); j++)
        aleds[s[2][0] + j] = CRGB(dmx.r3 + (ratio1 * j), dmx.g3 + (ratio2 * j), dmx.b3 + (ratio3 * j));

      ratio1 = (dmx.r5 - dmx.r4) / (float)(s[3][1] - s[3][0]);
      ratio2 = (dmx.g5 - dmx.g4) / (float)(s[3][1] - s[3][0]);
      ratio3 = (dmx.b5 - dmx.b4) / (float)(s[3][1] - s[3][0]);
      for(int j = 0; j <= (s[3][1] - s[3][0]); j++)
        aleds[s[3][0] + j] = CRGB(dmx.r4 + (ratio1 * j), dmx.g4 + (ratio2 * j), dmx.b4 + (ratio3 * j));
      
      ratio1 = (dmx.r6 - dmx.r5) / (float)(s[4][1] - s[4][0]);
      ratio2 = (dmx.g6 - dmx.g5) / (float)(s[4][1] - s[4][0]);
      ratio3 = (dmx.b6 - dmx.b5) / (float)(s[4][1] - s[4][0]);
      for(int j = 1; j <= (s[4][1] - s[4][0]); j++)
        aleds[s[4][1] - j] = CRGB(dmx.r5 + (ratio1 * j), dmx.g5 + (ratio2 * j), dmx.b5 + (ratio3 * j));

      ratio1 = (dmx.r1 - dmx.r6) / (float)(s[5][1] - s[5][0]);
      ratio2 = (dmx.g1 - dmx.g6) / (float)(s[5][1] - s[5][0]);
      ratio3 = (dmx.b1 - dmx.b6) / (float)(s[5][1] - s[5][0]);
      for(int j = 0; j < (s[5][1] - s[5][0]); j++)
        aleds[s[5][0] + j] = CRGB(dmx.r6 + (ratio1 * j), dmx.g6 + (ratio2 * j), dmx.b6 + (ratio3 * j));
    }

    // DIMMER MODE EFFECTS ------------------------------------
    
    if(dmx.dimmerMode <= 9) {
      // Right to Left
      dimmerRightToLeft( );
    }
    else if((dmx.dimmerMode >= 10 ) && (dmx.dimmerMode <= 19)) {
      // Up to Down
      dimmerUpToDown( );
    }
    else if((dmx.dimmerMode >= 20 ) && (dmx.dimmerMode <= 29)) {
      // up to down continuous
      
    }
    else {
      FastLED.clear( );
    }


    if(dmx.aMode <= 9) {
      // OFF
    }
    if((dmx.aMode >= 10) && (dmx.aMode <= 19)) {
      if(currentAMode != dmx.aMode) raceDown(true, dmx.aParameter1);
      else raceDown(false, dmx.aParameter1);
    }
    else if((dmx.aMode >= 20) && (dmx.aMode <= 29)) {
      if(currentAMode != dmx.aMode) sparkle(true, dmx.rA, dmx.gA, dmx.bA, dmx.aParameter1, dmx.aParameter2);
      else sparkle(false, dmx.rA, dmx.gA, dmx.bA, dmx.aParameter1, dmx.aParameter2);
    }
    else if((dmx.aMode >= 30) && (dmx.aMode <= 39)) {
      if(currentAMode != dmx.aMode) race3Orbits(true, dmx.aParameter1, dmx.aParameter2, dmx.aParameter3);
      else race3Orbits(false, dmx.aParameter1, dmx.aParameter2, dmx.aParameter3);
    }
    else if((dmx.aMode >= 40) && (dmx.aMode <= 49)) {
      if(currentAMode != dmx.aMode) raceTribits(true);
      else raceTribits(false);
    }

    currentColorMode = dmx.colorMode;
    currentDimmerMode = dmx.dimmerMode;
    currentAMode = dmx.aMode;

    // following is just testing rgbToHSV( ) algorithm
    //aleds[0] = rgbToHSV(CRGB(dmx.rA, dmx.gA, dmx.bA));

    FastLED.show( );
        
    updateMicrosSinceLastReading -= updateMicros;
  }

  microsPrevious = microsNow;
}

void raceTribits(bool isNew) {
  static int raceDownIndex;
  if(isNew) raceDownIndex = 0;

  raceStrip(true, 0, raceDownIndex);
  raceStrip(true, 1, raceDownIndex);
  raceStrip(true, 2, raceDownIndex);
  raceStrip(false, 3, raceDownIndex);
  raceStrip(false, 4, raceDownIndex);
  raceStrip(false, 5, raceDownIndex);
  if(++raceDownIndex >= 80) raceDownIndex = 0;
}

void raceDown(bool isNew, int velocity) {
  static float raceDownIndex;
  if(isNew) {
    if(velocity > 128) raceDownIndex = 0;
    else raceDownIndex = 80;
    for(int i = 0; i < 6; i++) raceStrip(true, i, (int)raceDownIndex);
    //return;
  }

  velocity -= 128;
  float relativeVelocityScale = (velocity / 128.0) * 5.7; // 5.x is the scaling factor to how quickly the exp func moves
  float relativeY;
  // piece wise function, bellow 2 is linear, above, it is exp.
  if((relativeVelocityScale >= 0.0) && (relativeVelocityScale <= 2.0)) {
    relativeY = -relativeVelocityScale + 2.0;
  }
  else if((relativeVelocityScale >= -2.0) && (relativeVelocityScale <= 0.0)) {
    relativeY = -(relativeVelocityScale + 2.0);
  }
  else {
    relativeY = 1.0 / (relativeVelocityScale * relativeVelocityScale);
    if(relativeVelocityScale < 0.0) relativeY = -relativeY;
  }
  float v = (relativeY / 2.0) * 10000000; // 10,000,000 aka 10 sec period is the max
  
  raceDownIndex += (80.0 * (microsThisReading / (float)v));

  if(raceDownIndex >= 80.0) {
    raceDownIndex = (int)raceDownIndex % 80;
    return;
  }
  else if(raceDownIndex < 0.0) {
    raceDownIndex = 80 - ((int)raceDownIndex % 80);
  }

  for(int i = 0; i < 6; i++) {
    raceStrip(true, i, (int)raceDownIndex);
  }
}

void race3Orbits(bool isNew, int velocity1, int velocity2, int velocity3) {
  static uint16_t orbit1Length = s[0][1] + s[3][1] - s[3][0];
  static uint16_t orbit2Length = s[1][1] - s[1][0] + s[4][1] - s[4][0];
  static uint16_t orbit3Length = s[2][1] - s[2][0] + s[5][1] - s[5][0];
  
  static float orbit1Index, orbit2Index, orbit3Index;
  
  if(isNew) {
    orbit1Index = 0;
    orbit2Index = 0;
    orbit3Index = 0;
  }

  if(!(dmx.dA | dmx.rA | dmx.gA | dmx.bA)) return;

  int tailLength = (dmx.aParameter4 / 1.5);
  int tailIndex;

  CRGB r = CRGB(scale8(dmx.dA, dmx.rA), scale8(dmx.dA, dmx.gA), scale8(dmx.dA, dmx.bA));
  CHSV h = rgbToHSV(r);
  h.s = 255;

  // ORBIT 1 --------------------
  velocity1 -= 128;
  float relativeVelocityScale = (velocity1 / 128.0) * 5.7; // 5.x is the scaling factor to how quickly the exp func moves
  float relativeY;
  // piece wise function, bellow 2 is linear, above, it is exp.
  if((relativeVelocityScale >= 0.0) && (relativeVelocityScale <= 2.0)) {
    relativeY = -relativeVelocityScale + 2.0;
  }
  else if((relativeVelocityScale >= -2.0) && (relativeVelocityScale <= 0.0)) {
    relativeY = -(relativeVelocityScale + 2.0);
  }
  else {
    relativeY = 1.0 / (relativeVelocityScale * relativeVelocityScale);
    if(relativeVelocityScale < 0.0) relativeY = -relativeY;
  }
  float v1 = (relativeY / 2.0) * 10000000; // 10,000,000 aka 10 sec period is the max

  orbit1Index += (orbit1Length * (microsThisReading / (float)v1));
  if(orbit1Index >= (float)orbit1Length) {
    orbit1Index = (int)orbit1Index % orbit1Length;
    //return;
  }
  else if(orbit1Index < 0.0) {
    orbit1Index = orbit1Length - ((int)-orbit1Index % orbit1Length);
  }

  // do Tail
  for(int i = 0; i < tailLength; i++) {
    if(v1 > 0.0) tailIndex = orbit1Index - i;
    else tailIndex = orbit1Index + i;
    if(tailIndex >= orbit1Length) {
      tailIndex = (int)tailIndex % orbit1Length;
    }
    else if(tailIndex < 0.0) {
      tailIndex = orbit1Length - ((int)-tailIndex % orbit1Length);
    }

    h.s = 255 - (i * 2);
    h.h += 7;
    if(tailIndex < (s[0][1] - s[0][0])) {
      aleds[s[0][0] + tailIndex] = h;
      //aleds[s[0][0] + tailIndex] = CRGB(scale8(dmx.dA, dmx.rA), scale8(dmx.dA, dmx.gA), scale8(dmx.dA, dmx.bA));
    }
    else
      aleds[s[3][0] + (tailIndex - s[0][1])] = h;
      //aleds[s[3][0] + (tailIndex - s[0][1])] = CRGB(scale8(dmx.dA, dmx.rA), scale8(dmx.dA, dmx.gA), scale8(dmx.dA, dmx.bA));
  }

  // ORBIT 2 --------------------
  velocity2 -= 128;
  relativeVelocityScale = (velocity2 / 128.0) * 5.7; // 5.x is the scaling factor to how quickly the exp func moves
  // piece wise function, bellow 2 is linear, above, it is exp.
  if((relativeVelocityScale >= 0.0) && (relativeVelocityScale <= 2.0)) {
    relativeY = -relativeVelocityScale + 2.0;
  }
  else if((relativeVelocityScale >= -2.0) && (relativeVelocityScale <= 0.0)) {
    relativeY = -(relativeVelocityScale + 2.0);
  }
  else {
    relativeY = 1.0 / (relativeVelocityScale * relativeVelocityScale);
    if(relativeVelocityScale < 0.0) relativeY = -relativeY;
  }
  float v2 = (relativeY / 2.0) * 10000000; // 10,000,000 aka 10 sec period is the max
  
  orbit2Index += (orbit2Length * (microsThisReading / (float)v2));
  if(orbit2Index >= (float)orbit2Length) {
    orbit2Index = (int)orbit2Index % orbit2Length;
    //return;
  }
  else if(orbit2Index < 0.0) {
    orbit2Index = orbit2Length - ((int)-orbit2Index % orbit2Length);
  }

  // do Tail
  h.h += 50;
  for(int i = 0; i < tailLength; i++) {
    if(v2 > 0.0) tailIndex = orbit2Index - i;
    else tailIndex = orbit2Index + i;
    if(tailIndex >= orbit2Length) {
      tailIndex = (int)tailIndex % orbit2Length;
    }
    else if(tailIndex < 0.0) {
      tailIndex = orbit2Length - ((int)-tailIndex % orbit2Length);
    }

    h.s = 255 - (i * 3);
    //h.h += 10;
    if(tailIndex < (s[1][1] - s[1][0])) {
      aleds[s[1][0] + tailIndex] = h;
    }
    else
      aleds[s[4][0] + (tailIndex - (s[1][1] - s[1][0]))] = h;
  }

  // ORBIT 3 --------------------
  velocity3 -= 128;
  relativeVelocityScale = (velocity3 / 128.0) * 5.7; // 5.x is the scaling factor to how quickly the exp func moves
  // piece wise function, bellow 2 is linear, above, it is exp.
  if((relativeVelocityScale >= 0.0) && (relativeVelocityScale <= 2.0)) {
    relativeY = -relativeVelocityScale + 2.0;
  }
  else if((relativeVelocityScale >= -2.0) && (relativeVelocityScale <= 0.0)) {
    relativeY = -(relativeVelocityScale + 2.0);
  }
  else {
    relativeY = 1.0 / (relativeVelocityScale * relativeVelocityScale);
    if(relativeVelocityScale < 0.0) relativeY = -relativeY;
  }
  float v3 = (relativeY / 2.0) * 10000000; // 10,000,000 aka 10 sec period is the max
  
  orbit3Index += (orbit3Length * (microsThisReading / (float)v3));
  if(orbit3Index >= (float)orbit3Length) {
    orbit3Index = (int)orbit3Index % orbit3Length;
    //return;
  }
  else if(orbit3Index < 0.0) {
    orbit3Index = orbit3Length - ((int)-orbit3Index % orbit3Length);
  }

  // do Tail
  h.h += 50;
  for(int i = 0; i < tailLength; i++) {
    if(v3 > 0.0) tailIndex = orbit3Index - i;
    else tailIndex = orbit3Index + i;
    if(tailIndex >= orbit3Length) {
      tailIndex = (int)tailIndex % orbit3Length;
    }
    else if(tailIndex < 0.0) {
      tailIndex = orbit3Length - ((int)-tailIndex % orbit3Length);
    }

    h.s = 255 - (i * 2);
    h.h += 2;
    if(tailIndex < (s[2][1] - s[2][0])) {
      aleds[s[2][0] + tailIndex] = h;
    }
    else
      aleds[s[5][0] + (tailIndex - (s[2][1] - s[2][0]))] = h;
  }

  // Map orbit index to strips

//  if(orbit1Index < s[0][1]) raceStrip(true, 0, orbit1Index);
//  else raceStrip(false, 3, orbit1Index - s[0][1]);

//  if(orbit2Index < (s[1][1] - s[1][0])) raceStrip(true, 1, orbit2Index);
//  else raceStrip(false, 4, orbit2Index - (s[1][1] - s[1][0]));

//  if(orbit3Index < (s[2][1] - s[2][0])) raceStrip(true, 2, orbit3Index);
//  else raceStrip(false, 5, orbit3Index - (s[2][1] - s[2][0]));
}

void raceStrip(bool directionDown, int strip, int index) {
  if(!(dmx.dA | dmx.rA | dmx.gA | dmx.bA)) return;
  if(directionDown) {
    if(strip < 3) aleds[s[strip][0] + index] = CRGB(scale8(dmx.dA, dmx.rA), scale8(dmx.dA, dmx.gA), scale8(dmx.dA, dmx.bA));
    else aleds[s[strip][1] - index] = CRGB(scale8(dmx.dA, dmx.rA), scale8(dmx.dA, dmx.gA), scale8(dmx.dA, dmx.bA));
  } else {
    if(strip < 3) aleds[s[strip][1] - index] = CRGB(scale8(dmx.dA, dmx.rA), scale8(dmx.dA, dmx.gA), scale8(dmx.dA, dmx.bA));
    else aleds[s[strip][0] + index] = CRGB(scale8(dmx.dA, dmx.rA), scale8(dmx.dA, dmx.gA), scale8(dmx.dA, dmx.bA));
  }
}


void cometStrip(bool directionDown, int strip, int index, int tailLength) {
  if(!(dmx.dA | dmx.rA | dmx.gA | dmx.bA)) return;
  if(directionDown) {
    if(strip < 3) {
      for(int i = 0; i < tailLength; i++) {
        if((index - i) >= 0)
          aleds[s[strip][0] + index - i] = CRGB(scale8(dmx.dA, dmx.rA), scale8(dmx.dA, dmx.gA), scale8(dmx.dA, dmx.bA));
      }
    }
    else {
      for(int i = 0; i < tailLength; i++) {
        if((s[strip][1] - index + i) > s[strip][1]) 
          aleds[s[strip][1] - index + i] = CRGB(scale8(dmx.dA, dmx.rA), scale8(dmx.dA, dmx.gA), scale8(dmx.dA, dmx.bA));
      }
    }
  } else {
    if(strip < 3) aleds[s[strip][1] - index] = CRGB(scale8(dmx.dA, dmx.rA), scale8(dmx.dA, dmx.gA), scale8(dmx.dA, dmx.bA));
    else aleds[s[strip][0] + index] = CRGB(scale8(dmx.dA, dmx.rA), scale8(dmx.dA, dmx.gA), scale8(dmx.dA, dmx.bA));
  }
}

void sparkle(bool isNew, byte red, byte green, byte blue, uint8_t numSparkles, uint8_t delayTime) {
  static int Pixels[256];
  
  static uint8_t delayCount = 0;
  if(isNew) {
    delayCount = 0;
  }

  if(delayCount++ >= delayTime) {
    delayCount = 0;
  } else return;

  if(numSparkles >= 128) {
    fill_solid(aleds, ALED_COUNT, CRGB(scale8(dmx.dA, red), scale8(dmx.dA, green), scale8(dmx.dA, blue)));
    for(int i = 0; i < (512 - ((numSparkles - 128) * 4)); i++) aleds[random16(ALED_COUNT)] =  CRGB(0, 0, 0);
    return;
  }
  
  for(int i = 0; i < numSparkles; i++) {
    Pixels[i] = random16(ALED_COUNT);
    aleds[Pixels[i]] =  CRGB(scale8(dmx.dA, dmx.rA), scale8(dmx.dA, dmx.gA), scale8(dmx.dA, dmx.bA));
  }
}

float Min(float fR, float fG, float fB) {
  float fMin = fR;
  if(fG < fMin) fMin = fG;
  if(fB < fMin) fMin = fB;
  return fMin;
}


float Max(float fR, float fG, float fB) {
  float fMax = fR;
  if(fG > fMax) fMax = fG;
  if(fB > fMax) fMax = fB;
  return fMax;
}

CHSV rgbToHSV(CRGB color) {
  float rP = color.r / 255.0;
  float gP = color.g / 255.0;
  float bP = color.b / 255.0;

  float cMax = Max(rP, gP, bP);
  float cMin = Min(rP, gP, bP);

  float delta = cMax - cMin;
  float hue;
  
  if(delta == 0.0) {
    hue = 0;
    //Serial.print("cMax: "); Serial.print(cMax); Serial.print(" cMin:"); Serial.println(cMin);
    return CHSV(0, 0, ((cMax + cMin) / 2.0) * 255);
  }
  else if(cMax == rP) {
    hue = 60.0 * fmod(((gP - bP) / delta), 6.0);
    if(hue < 0.0) hue = 360.0 + hue;
  }
  else if(cMax == gP) {
    hue = 60.0 * (((bP - rP) / delta) + 2.0);
  }
  else {
  //else if(cMax == bP) {
    hue = 60.0 * (((rP - gP) / delta) + 4.0);
  }

  float lightness = ((cMax + cMin) / 2.0);
  float saturation = (delta / (1 - fabs(2.0 * lightness - 1.0)));

//  Serial.print("cMax: "); Serial.print(cMax); Serial.print(" cMin:"); Serial.print(cMin); Serial.print(" : ");
//  Serial.print(hue); Serial.print(" - "); Serial.print(saturation); Serial.print(" - "); Serial.println(lightness);

  return CHSV(hue * (255.0 / 360.0), saturation * 255, lightness * 255);
}

CHSV rgbToHSV_old(CRGB color) {
  float rP = color.r / 255.0;
  float gP = color.g / 255.0;
  float bP = color.b / 255.0;

  float cMax, cMin;
  uint8_t cMaxUnit, cMinUnit;

  if(rP > gP) {
    if(rP > bP) {
      // rP is greatest
      cMax = rP;
      cMaxUnit = 0;

      if(bP > gP) {
        cMin = gP;
        cMinUnit = 1;
      }
      else {
        cMin = bP;
        cMinUnit = 2;
      }
    }
    else {
      // rP is second
      // bP is greatest
      cMax = bP;
      cMaxUnit = 2;
      // gP is min
      cMin = gP;
      cMinUnit = 1;
    }
  }
  else {
    if(gP > bP) {
      cMax = gP;
      cMaxUnit = 1;

      if(bP > rP) {
        cMin = rP;
        cMinUnit = 0;
      }
      else {
        cMin = bP;
        cMinUnit = 2;
      }
    }
    else {
      cMax = bP;
      cMaxUnit = 2;

      if(rP < gP) {
        cMin = rP;
        cMinUnit = 0;
      }
      else {
        cMin = gP;
        cMinUnit = 1;
      }
    }
  }

  float delta = cMax - cMin;
  if(delta == 0.0) cMaxUnit = 5;

  uint8_t hue;
  
  switch(cMaxUnit) {
    case 0:
      hue = 60.0 * (((gP - bP) / delta) + (gP < bP ? 6 : 0));
      break;
    case 1:
      hue = 60.0 * (((bP - rP) / delta) + 2);
      break;
    case 2:
      hue = 60.0 * (((rP - gP) / delta) + 4);
      break;
    case 5:
      hue = 0;
    default:
      break;
  }

  uint8_t lightness = (cMax + cMin) / 2;
  uint8_t saturation = delta / (1 - abs(2 * lightness - 1));

  Serial.print("cMax: "); Serial.print(cMax); Serial.print(" cMin:"); Serial.print(cMin); Serial.print(" : ");
  Serial.print(hue); Serial.print(" - "); Serial.print(saturation); Serial.print(" - "); Serial.println(lightness);

  return CHSV(hue, saturation, lightness);
}

void serialAssemblePacket( )
{
  unsigned char bytesRead = Serial1.readBytes(serialBuffer, serialBufferSize);

/*
  if(bytesRead > 0) {
    Serial.print("READ = ");
    Serial.print(bytesRead);
    Serial.print(" = ");
  }
  else return; */
  
  for(int i = 0; i < bytesRead; i++) {
    //Serial.print(serialBuffer[i], HEX); Serial.print(" ");
    switch(serialPacketState) {
      case 0:
        if(serialBuffer[i] == 0xFA) serialPacketState = 1;
        break;
      case 1:
        if(serialBuffer[i] == 0x12) serialPacketState = 2;
        else serialPacketState = 0;
        break;
      case 2:
        if(serialBuffer[i] == 0xA7) serialPacketState = 3;
        else serialPacketState = 0;
        break;
      case 3:
        serialPacketType = serialBuffer[i];
        serialPacketIndex = 0;
        switch(serialBuffer[i]) {
          case GUART_MSG_DMX_UPDATE:
            serialBytesExpected = GUART_MSG_DMX_UPDATE_SIZE;
            serialPacketState = 4;
            break;
          default:
            // Unknown packet type, so we must invalidate the packet
            serialPacketState = 0;
        }
        break;
      case 4:
        if(serialBytesExpected--) {
          serialPacket[serialPacketIndex++] = serialBuffer[i];
          //Serial.print(serialBuffer[i]); Serial.print(" ");
        }
        if(serialBytesExpected == 0) {
          // Packet is complete, deal with it
          switch(serialPacketType) {
            case GUART_MSG_DMX_UPDATE:
              dmx = *((GUARTmsgDMXUpdate *)serialPacket);

              //Serial.println("FULL PACKET");
              break;
            default: // This case should never happen since in state 3 we check for validity
              break;
          }
          serialPacketState = 0;
        }
        break;
      default: // This case should never happen
        break;
    } /* switch(serialPacketState) */
  } /* for(i) */

  //Serial.println("");
}

/* eof */
