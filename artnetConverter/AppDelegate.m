//
//  AppDelegate.m
//  artnetConverter
//
//  Created by Richard Arima on 7/23/19.
//  Copyright © 2019 Richard Arima. All rights reserved.
//

#import "AppDelegate.h"
#import "LEDPackets.h"

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/_select.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

@interface AppDelegate ()

@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // Insert code here to initialize your application
    _nc = [NSNotificationCenter defaultCenter];
    [_nc addObserver:self selector:@selector(discoMIDIChannelChanged:) name:@"discoMIDIChannelChanged" object:nil];
    
    discoMIDIChannel = 1;
    [self setDiscoMIDIChannel:DISCO_MIDI_CHANNEL];
    
    _serial = [[SerialConnector alloc] init];
    if(_serial == nil) {
        NSLog(@"SerialConnector could not be created");
        return;
    }
    NSLog(@"SerialConnector created");
    
//    _peripheral = [[BLEPeripheral alloc] init];
//    if(_peripheral == nil) {
//        NSLog(@"BLEPeripheral could not be created");
//        return;
//    }
//    NSLog(@"BLEPeripheral started");
    
    _netLoopThread = [[NSThread alloc] initWithTarget:self selector:@selector(netLoop) object:nil];
    [_netLoopThread start];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication {
    return YES;
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
    if(_netLoopThread) {
        if([_netLoopThread isExecuting]) {
            [_netLoopThread cancel];
            close(sock);
            _netLoopThread = nil;
        }
    }
    
    if(_serial) [_serial shutDown];
//    if(_peripheral) {
//        [_peripheral shutDown];
//        _peripheral = nil;
//    }
}

- (BOOL)setDiscoMIDIChannel:(int)channel {
    if((channel < 1) || ((channel + GUART_MSG_DMX_UPDATE_SIZE) > 512)) {
        [_nc postNotificationName:@"discoMIDIChannelChanged" object:self userInfo:@{ @"channel" : [NSNumber numberWithInt:discoMIDIChannel] }];
        return NO;
    }
    
    discoMIDIChannel = channel;
    [_nc postNotificationName:@"discoMIDIChannelChanged" object:self userInfo:@{ @"channel" : [NSNumber numberWithInt:discoMIDIChannel] }];
    
    return YES;
}

- (void)netLoop {
    sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(sock == -1) {
        printf("Could not get socket");
        return;
    }
    
    struct hostent *hn;
    //hn = gethostbyname("10.0.1.21");
    hn = gethostbyname("127.0.0.1");
    if(hn == NULL) {
        printf("Error resolving hostname");
        close(sock);
        return;
    }
    
    int option = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option))) {
        printf("Error: listenForPackets - setsockopt failed REUSEADDR");
        close(sock);
        return;
    }
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &option, sizeof(option))) {
        printf("Error: listenForPackets - setsockopt failed REUSEPORT");
        close(sock);
        return;
    }
    
    struct sockaddr_in     addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = PF_INET;
    addr.sin_port = htons(6454);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if(bind(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        printf("Bind error");
    }
    
    addr.sin_addr = *((struct in_addr *)*hn->h_addr_list);
    
    struct timeval  tout = { 2, 0 };
    fd_set          rfds;
    //int             ntorecv;
    int             err = 1;
    
    tout.tv_sec = 2;
    tout.tv_usec = 0;
    
    while(![self.netLoopThread isCancelled]) {
        FD_ZERO(&rfds);
        FD_SET(sock, &rfds);
        err = select(sock + 1, &rfds, NULL, NULL, &tout);
        struct sockaddr_in i_addr;
        int i_len = sizeof(i_addr);
        u_char read_buffer[4096] = { 0 };
        
        int nread = (int)recvfrom(sock, (char *)read_buffer, 4096, MSG_DONTWAIT, (struct sockaddr *)&i_addr, (socklen_t *)&i_len);
        
//        printf("%d\n", nread);
//        printf("%c %c %c %c %c %c %c %d ", read_buffer[0], read_buffer[1], read_buffer[2], read_buffer[3], read_buffer[4], read_buffer[5], read_buffer[6], read_buffer[7]);
//        printf("%d %d, %d %d, %d %d ", *((unsigned short *)&read_buffer[8]), *((unsigned short *)&read_buffer[10]), read_buffer[12], read_buffer[13], *((unsigned short *)&read_buffer[14]), *((unsigned short *)&read_buffer[16]));
//        printf("\nSTART\n");
//        for(int i = 0; i < (nread - 18); i++) {
//            printf("%d ", read_buffer[i + 18]);
//        }
//        printf("\n");
        
        if(nread == 530) {
//            if(read_buffer[17 + discoMIDIChannel] != discoBrightness) {
//                discoBrightness = read_buffer[17 + discoMIDIChannel];
//                [_nc postNotificationName:@"discoBrightnessChanged" object:[NSNumber numberWithInt:discoBrightness]];
//            }
//            if(read_buffer[18 + discoMIDIChannel] != discoMode) {
//                discoMode = read_buffer[18 + discoMIDIChannel];
//                [_nc postNotificationName:@"discoModeChanged" object:[NSNumber numberWithInt:discoMode]];
//            }
//            if(read_buffer[19 + discoMIDIChannel] != discoSpeed) {
//                discoSpeed = read_buffer[19 + discoMIDIChannel];
//                [_nc postNotificationName:@"discoSpeedChanged" object:[NSNumber numberWithInt:discoSpeed]];
//            }
            currentDMX = *((GUARTmsgDMXUpdate *)&read_buffer[17 + discoMIDIChannel]);
            //NSLog(@"%d %d", currentDMX.masterDimmer, currentDMX.dimmerMode);
            
            [_nc postNotificationName:@"discoDMXChanged" object:self userInfo:@{ @"dmxData" : [NSData dataWithBytes:&currentDMX length:sizeof(GUARTmsgDMXUpdate)] }];
            
            if(![_serial sendPacketType:GUART_MSG_DMX_UPDATE WithBuffer:(uint8_t *)&read_buffer[17 + discoMIDIChannel]])
            {
                NSLog(@"Error writing to serial port");
            }
        }
    }
    
    //close(sock);
}

- (void)discoMIDIChannelChanged:(NSNotification *)n {
    if([n object] == self) return;
    int channel = [[n userInfo][@"channel"] intValue];
    [self setDiscoMIDIChannel:channel];
}

@end
