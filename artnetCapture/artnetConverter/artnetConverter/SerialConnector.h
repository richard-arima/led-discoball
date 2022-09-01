//
//  SerialConnector.h
//  GBase
//
//  Created by Richard Arima on 1/17/18.
//  Copyright © 2018 Richard Arima. All rights reserved.
//

#ifndef SerialConnector_h
#define SerialConnector_h

#import <Foundation/Foundation.h>
#import <ORSSerial/ORSSerial.h>
#import <ORSSerial/ORSSerialPort.h>

@interface SerialConnector : NSObject <ORSSerialPortDelegate> {
    ORSSerialPort *port;
    NSNotificationCenter *nc;
    
    NSDictionary *packetTypesAndSizes;
    NSDictionary *currentPacketDicInfo;
    long currentPacketSize;
    
    int serialPacketState;
    uint8_t serialPacketType;
    char *serialBuffer, serialPacket[1024];
    char serialOutBuffer[1024];
    unsigned long bytesRead, serialPacketIndex, serialBytesExpected;
    
    BOOL connectedReceivingData;
    uint64_t latestPacketTime;
    NSTimer *connectionStatusTimer;
}

// internal functions
- (void)openPort;
- (void)assemblePacket;
- (void)startRetryTimer;
- (void)shutDown;
- (BOOL)sendPacketType:(int)packetType WithBuffer:(uint8_t *)buffer;

@end

#endif /* SerialConnector_h */

