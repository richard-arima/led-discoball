// LED Controller Packets


#ifndef __LED_PACKETS__H__
#define __LED_PACKETS__H__

#include <stdint.h>

#define GUART_MSG_HEADER_SIZE           4

typedef struct _GUART_Packet {
    uint8_t header0;            // 0xFA
    uint8_t header1;            // 0x12
    uint8_t header2;            // 0xA7
    uint8_t packetType;
    uint8_t *data;
} GUART_Packet;

// Serial Protocol (UART) Packet Types
//#define UART_MSG_DMX_UPDATE             1   
//#define UART_MSG_DMX_UPDATE_SIZE        4
//typedef struct _UARTmsgDMXUpdate {
//    uint8_t ledBrightness, ledMode, ledSpeed, pad1;
//} UARTmsgDMXUpdate;

#define GUART_MSG_DMX_UPDATE             2
#define GUART_MSG_DMX_UPDATE_SIZE        36
typedef struct _GUARTmsgDMXUpdate {
    uint8_t masterDimmer;
    uint8_t dimmerMode;
    uint8_t d1, r1, g1, b1;
    uint8_t d2, r2, g2, b2;
    uint8_t d3, r3, g3, b3;
    uint8_t d4, r4, g4, b4;
    uint8_t d5, r5, g5, b5;
    uint8_t d6, r6, g6, b6;
    uint8_t colorMode;
    uint8_t dA, rA, gA, bA;
    uint8_t aMode, aParameter1, aParameter2, aParameter3, aParameter4;
} GUARTmsgDMXUpdate;

#ifdef __APPLE__

extern NSString *const GMSGserialConnectedStatus;

extern NSString *const GMSGpacketSize;
extern NSString *const GMSGnotification;

extern NSString *const GMSGdmxUpdate;

#define GUART_MSG_PACKETS_AND_SIZES_NSDICTIONARY @{ \
@GUART_MSG_DMX_UPDATE : @{ \
GMSGpacketSize: @GUART_MSG_DMX_UPDATE_SIZE, GMSGnotification : GMSGdmxUpdate } \
}

#endif /* __APPLE__ */

#endif /* __LED_PACKETS__H__ */
