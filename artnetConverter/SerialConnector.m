//
//  SerialConnector.m
//  GBase
//
//  Created by Richard Arima on 1/17/18.
//  Copyright © 2018 Richard Arima. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <ORSSerial/ORSSerial.h>
#import <ORSSerial/ORSSerialPort.h>
#include <mach/mach.h>

#import "LEDPackets.h"
#import "SerialConnector.h"

@implementation SerialConnector

- (id)init
{
    self = [super init];
    if(self) {
        connectedReceivingData = NO;
        packetTypesAndSizes = GUART_MSG_PACKETS_AND_SIZES_NSDICTIONARY;
        connectionStatusTimer = [NSTimer timerWithTimeInterval:0.3 target:self selector:@selector(connectionStatusTimerHandler:) userInfo:NULL repeats:YES];
        [[NSRunLoop currentRunLoop] addTimer:connectionStatusTimer forMode:NSDefaultRunLoopMode];
        latestPacketTime = mach_absolute_time( );
        [self openPort];
    }
    
    return self;
}

- (void)openPort
{
    serialPacketState = 0;
    serialPacketType = 0;
    nc = [NSNotificationCenter defaultCenter];
    
    port = [ORSSerialPort serialPortWithPath:@"/dev/cu.usbserial-DN02LNBA"];
    if(port == nil) {
        [self startRetryTimer];
        NSLog(@"SerialConnector could not open port, retrying in 2 seconds");
        return;
    }
    port.baudRate = @115200;
    port.parity = ORSSerialPortParityNone;
    port.numberOfStopBits = 1;
    port.usesRTSCTSFlowControl = NO;
    port.delegate = self;
    [port open];
    
    [nc postNotificationName:GMSGserialConnectedStatus object:[NSNumber numberWithInt:1]];
}

- (void)connectionStatusTimerHandler:(NSTimer *)timer {
    uint64_t now = mach_absolute_time( );
    if((now - latestPacketTime) > 100000000) {
        connectedReceivingData = NO;
        if(port != nil) [nc postNotificationName:GMSGserialConnectedStatus object:[NSNumber numberWithInt:1]];
    }
}

- (void)retryTimerStart:(NSTimer *)timer {
    [self openPort];
}

- (void)startRetryTimer {
    [nc postNotificationName:GMSGserialConnectedStatus object:[NSNumber numberWithInt:0]];
    
    NSTimer *retryTimer = [NSTimer timerWithTimeInterval:2.0 target:self selector:@selector(retryTimerStart:) userInfo:NULL repeats:NO];
    [[NSRunLoop currentRunLoop] addTimer:retryTimer forMode:NSDefaultRunLoopMode];
}

- (void)assemblePacket
{
    for(int i = 0; i < bytesRead; i++) {
        switch(serialPacketState) {
            case 0:
                if(serialBuffer[i] == (char)0xFA) serialPacketState = 1;
                break;
            case 1:
                if(serialBuffer[i] == (char)0x12) serialPacketState = 2;
                else serialPacketState = 0;
                break;
            case 2:
                if(serialBuffer[i] == (char)0xA7) serialPacketState = 3;
                else serialPacketState = 0;
                break;
            case 3: {
                serialPacketType = serialBuffer[i];
                serialPacketIndex = 0;
                
                currentPacketDicInfo = [packetTypesAndSizes objectForKey:[NSNumber numberWithInt:serialBuffer[i]]];
                if(currentPacketDicInfo == nil) {
                    NSLog(@"PACKET ASSEMBLY ERROR ----- STAGE 1!!!!!! serialPacketType: %d", serialPacketType);
                    serialPacketState = 0;
                    goto SKIP;
                }
                NSNumber *n = [currentPacketDicInfo objectForKey:GMSGpacketSize];
                serialBytesExpected = [n intValue];
                currentPacketSize = serialBytesExpected;
                
                serialPacketState = 4;
                break;
            }
            SKIP:
            case 4:
                if(serialBytesExpected--) serialPacket[serialPacketIndex++] = serialBuffer[i];
                if(serialBytesExpected == 0) {
                    // Packet is complete, deal with it
                    
                    NSData *outData = [NSData dataWithBytes:serialPacket length:currentPacketSize];
                    [nc postNotificationName:[currentPacketDicInfo objectForKey:GMSGnotification] object:outData];
                    
                    serialPacketState = 0;
                }
                break;
            default: // This case should never happen
                break;
        } /* switch(serialPacketState) */
    } /* for(i) */
}

- (void)serialPort:(ORSSerialPort *)serialPort didReceiveData:(NSData *)data
{
    if(!connectedReceivingData) {
        [nc postNotificationName:GMSGserialConnectedStatus object:[NSNumber numberWithInt:2]];
        connectedReceivingData = YES;
    }
    latestPacketTime = mach_absolute_time( );
    serialBuffer = (char *)[data bytes];
    bytesRead = [data length];
    [self assemblePacket];
}

- (void)serialPortWasRemovedFromSystem:(ORSSerialPort *)serialPort
{
    connectedReceivingData = NO;
    [nc postNotificationName:GMSGserialConnectedStatus object:[NSNumber numberWithInt:0]];
    port = nil;
    [self startRetryTimer];
    
    NSLog(@"Serial port %@ was removed from system retrying in 2 secodnds", serialPort);
}

- (void)serialPort:(ORSSerialPort *)serialPort didEncounterError:(NSError *)error
{
    connectedReceivingData = NO;
    [nc postNotificationName:GMSGserialConnectedStatus object:[NSNumber numberWithInt:0]];
    [port close];
    port = nil;
    [self startRetryTimer];
    
    NSLog(@"Serial port %@ encountered an error: %@ retrying in 2 seconds", serialPort, error);
}

- (BOOL)sendPacketType:(int)packetType WithBuffer:(uint8_t *)buffer
{
    if(!port) return NO;
    
    long packetSize = 0;
    GUART_Packet *packet = (GUART_Packet *)serialOutBuffer;
    packet->header0 = 0xFA;
    packet->header1 = 0x12;
    packet->header2 = 0xA7;
    packet->packetType = packetType;
    
    NSDictionary *d = [packetTypesAndSizes objectForKey:[NSNumber numberWithInt:packetType]];
    if(d == nil) {
        NSLog(@"NetConnection: Unknown Packet Type, Packet Not Sent");
        serialPacketState = 0;
        return NO;
    }
    NSNumber *n = [d objectForKey:GMSGpacketSize];
    packetSize = [n intValue];
    
    memcpy(&serialOutBuffer[4], buffer, packetSize);
    packetSize += GUART_MSG_HEADER_SIZE;
    NSData *data = [[NSData alloc] initWithBytes:serialOutBuffer length:packetSize];
    
//    for(int i = 0; i < packetSize; i++) {
//        printf("%x ", serialOutBuffer[i]);
//    }
//    printf("\n");
    return [port sendData:data];
}

- (void)shutDown
{
    [connectionStatusTimer invalidate];
    connectedReceivingData = NO;
    [nc postNotificationName:GMSGserialConnectedStatus object:[NSNumber numberWithInt:0]];if(port) [port close];
    port = nil;
}

@end

