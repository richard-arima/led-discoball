//
//  ViewController.m
//  artnetConverter
//
//  Created by Richard Arima on 7/23/19.
//  Copyright © 2019 Richard Arima. All rights reserved.
//

#import "ViewController.h"
#import "LEDPackets.h"

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];

    _nc = [NSNotificationCenter defaultCenter];
//    [_nc addObserver:self selector:@selector(discoBrightnessChanged:) name:@"discoBrightnessChanged" object:nil];
//    [_nc addObserver:self selector:@selector(discoModeChanged:) name:@"discoModeChanged" object:nil];
//    [_nc addObserver:self selector:@selector(discoSpeedChanged:) name:@"discoSpeedChanged" object:nil];
    [_nc addObserver:self selector:@selector(discoMIDIChannelChanged:) name:@"discoMIDIChannelChanged" object:nil];
    [_nc addObserver:self selector:@selector(discoDMXChanged:) name:@"discoDMXChanged" object:nil];
    [_nc addObserver:self selector:@selector(serialStatusChanged:) name:GMSGserialConnectedStatus object:nil];
    
    [self serialStatusChanged:[NSNotification notificationWithName:GMSGserialConnectedStatus object:[NSNumber numberWithInt:0]]];
}
- (IBAction)discoMIDITextFieldChanged:(NSTextField *)sender {
    int channel = [_discoMIDIChannelTextField.stringValue intValue];
    [_nc postNotificationName:@"discoMIDIChannelChanged" object:self userInfo:@{ @"channel" : [NSNumber numberWithInt:channel] }];
}

- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}

//- (void)discoBrightnessChanged:(NSNotification *)n {
//    dispatch_queue_t dq = dispatch_get_main_queue( );
//    dispatch_async(dq, ^{
//        self->_discoBrightnessTextField.stringValue = [NSString stringWithFormat:@"%d", [[n object] intValue]];
//    });
//}
//
//- (void)discoModeChanged:(NSNotification *)n {
//    dispatch_queue_t dq = dispatch_get_main_queue( );
//    dispatch_async(dq, ^{
//        self->_discoModeTextField.stringValue = [NSString stringWithFormat:@"%d", [[n object] intValue]];
//    });
//}
//
//- (void)discoSpeedChanged:(NSNotification *)n {
//    dispatch_queue_t dq = dispatch_get_main_queue( );
//    dispatch_async(dq, ^{
//        self->_discoSpeedTextField.stringValue = [NSString stringWithFormat:@"%d", [[n object] intValue]];
//    });
//}

- (void)discoDMXChanged:(NSNotification *)n {
    if([n object] == self) return;
    GUARTmsgDMXUpdate *dmx = (GUARTmsgDMXUpdate *)[[n userInfo][@"dmxData"] bytes];
    
    dispatch_queue_t dq = dispatch_get_main_queue( );
    dispatch_async(dq, ^{
        self->_discoMasterDimmerTextField.stringValue = [NSString stringWithFormat:@"%d", dmx->masterDimmer];
        self->_discoDimmerModeTextField.stringValue = [NSString stringWithFormat:@"%d", dmx->dimmerMode];
        
        self->_disco1DTextField.stringValue = [NSString stringWithFormat:@"%d", dmx->d1];
        self->_disco1RTextField.stringValue = [NSString stringWithFormat:@"%d", dmx->r1];
        self->_disco1GTextField.stringValue = [NSString stringWithFormat:@"%d", dmx->g1];
        self->_disco1BTextField.stringValue = [NSString stringWithFormat:@"%d", dmx->b1];
        
        self->_disco2DTextField.stringValue = [NSString stringWithFormat:@"%d", dmx->d2];
        self->_disco2RTextField.stringValue = [NSString stringWithFormat:@"%d", dmx->r2];
        self->_disco2GTextField.stringValue = [NSString stringWithFormat:@"%d", dmx->g2];
        self->_disco2BTextField.stringValue = [NSString stringWithFormat:@"%d", dmx->b2];
        
        self->_disco3DTextField.stringValue = [NSString stringWithFormat:@"%d", dmx->d3];
        self->_disco3RTextField.stringValue = [NSString stringWithFormat:@"%d", dmx->r3];
        self->_disco3GTextField.stringValue = [NSString stringWithFormat:@"%d", dmx->g3];
        self->_disco3BTextField.stringValue = [NSString stringWithFormat:@"%d", dmx->b3];
        
        self->_disco4DTextField.stringValue = [NSString stringWithFormat:@"%d", dmx->d4];
        self->_disco4RTextField.stringValue = [NSString stringWithFormat:@"%d", dmx->r4];
        self->_disco4GTextField.stringValue = [NSString stringWithFormat:@"%d", dmx->g4];
        self->_disco4BTextField.stringValue = [NSString stringWithFormat:@"%d", dmx->b4];
        
        self->_disco5DTextField.stringValue = [NSString stringWithFormat:@"%d", dmx->d5];
        self->_disco5RTextField.stringValue = [NSString stringWithFormat:@"%d", dmx->r5];
        self->_disco5GTextField.stringValue = [NSString stringWithFormat:@"%d", dmx->g5];
        self->_disco5BTextField.stringValue = [NSString stringWithFormat:@"%d", dmx->b5];
        
        self->_disco6DTextField.stringValue = [NSString stringWithFormat:@"%d", dmx->d6];
        self->_disco6RTextField.stringValue = [NSString stringWithFormat:@"%d", dmx->r6];
        self->_disco6GTextField.stringValue = [NSString stringWithFormat:@"%d", dmx->g6];
        self->_disco6BTextField.stringValue = [NSString stringWithFormat:@"%d", dmx->b6];
        
        self->_discoColorModeTextField.stringValue = [NSString stringWithFormat:@"%d", dmx->colorMode];
        
        self->_discoADTextField.stringValue = [NSString stringWithFormat:@"%d", dmx->dA];
        self->_discoARTextField.stringValue = [NSString stringWithFormat:@"%d", dmx->rA];
        self->_discoAGTextField.stringValue = [NSString stringWithFormat:@"%d", dmx->gA];
        self->_discoABTextField.stringValue = [NSString stringWithFormat:@"%d", dmx->bA];
        self->_discoAModeTextField.stringValue = [NSString stringWithFormat:@"%d", dmx->aMode];
        self->_discoAParameter1TextField.stringValue = [NSString stringWithFormat:@"%d", dmx->aParameter1];
        self->_discoAParameter2TextField.stringValue = [NSString stringWithFormat:@"%d", dmx->aParameter2];
        self->_discoAParameter3TextField.stringValue = [NSString stringWithFormat:@"%d", dmx->aParameter3];
        self->_discoAParameter4TextField.stringValue = [NSString stringWithFormat:@"%d", dmx->aParameter4];
    });
}

- (void)discoMIDIChannelChanged:(NSNotification *)n {
    if([n object] == self) return;
    int channel = [[n userInfo][@"channel"] intValue];
    
    dispatch_queue_t dq = dispatch_get_main_queue( );
    dispatch_async(dq, ^{
        self->_discoMIDIChannelTextField.stringValue = [NSString stringWithFormat:@"%d", channel];
        [self.discoMIDIChannelTextField.window makeFirstResponder:nil];
    });
}

- (void)serialStatusChanged:(NSNotification *)n {
    NSNumber *num = [n object];
    switch([num intValue]) {
        case 2:
            _serialStatusOutputTextField.stringValue = @"Serial Receiving";
            _serialStatusOutputTextField.backgroundColor = [NSColor greenColor];
            break;
        case 1:
            _serialStatusOutputTextField.stringValue = @"Serial Connected";
            _serialStatusOutputTextField.backgroundColor = [NSColor greenColor];
            break;
        case 0:
            _serialStatusOutputTextField.stringValue = @"Serial Disconnected";
            _serialStatusOutputTextField.backgroundColor = [NSColor redColor];
            break;
        default:
            break;
    }
}

@end
