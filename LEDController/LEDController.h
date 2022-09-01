// LEDController.h
// common header for global variables

#ifndef __LED_CONTROLLER__H__
#define __LED_CONTROLLER__H__

#include "LEDPackets.h"

#include <FastLED.h>

#define POWER_ON_LED_PIN 13


#define ALED_COUNT 490
//#define ALED_COUNT 20
#define ALED_PIN 11

extern CRGB aleds[ALED_COUNT];

extern CRGBSet aledsSet;

extern const uint16_t s[6][2];

// Other Globals
extern unsigned long microsPrevious;
extern unsigned int rollOver;
//extern const uint16_t serialBufferSize = 256;
//extern char serialBuffer[serialBufferSize], serialPacket[serialBufferSize];
//extern uint8_t serialPacketState, serialBytesExpected, serialPacketType, serialPacketIndex;
extern unsigned int updateMicros, updateMicrosSinceLastReading;
extern unsigned long microsNow;
extern unsigned long microsThisReading;

extern uint8_t currentColorMode;
extern uint8_t currentDimmerMode;
extern uint8_t currentAMode;

extern GUARTmsgDMXUpdate dmx;

#endif /* __LED_CONTROLLER__H__ */
