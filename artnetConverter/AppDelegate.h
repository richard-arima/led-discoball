//
//  AppDelegate.h
//  artnetConverter
//
//  Created by Richard Arima on 7/23/19.
//  Copyright © 2019 Richard Arima. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "SerialConnector.h"
#import "LEDPackets.h"

//#import "BLEPeripheral.h"

#define DISCO_MIDI_CHANNEL  1

@interface AppDelegate : NSObject <NSApplicationDelegate> {
    int sock;
    int discoMIDIChannel;
    GUARTmsgDMXUpdate currentDMX;
}

@property (strong, nonatomic) NSThread *netLoopThread;
@property (strong, nonatomic) SerialConnector *serial;
//@property (strong, nonatomic) BLEPeripheral *peripheral;

@property (strong, nonatomic) NSNotificationCenter *nc;

@end

