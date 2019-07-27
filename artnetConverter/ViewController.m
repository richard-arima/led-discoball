//
//  ViewController.m
//  artnetConverter
//
//  Created by Richard Arima on 7/23/19.
//  Copyright © 2019 Richard Arima. All rights reserved.
//

#import "ViewController.h"

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];

    _nc = [NSNotificationCenter defaultCenter];
    [_nc addObserver:self selector:@selector(discoBrightnessChanged:) name:@"discoBrightnessChanged" object:nil];
    [_nc addObserver:self selector:@selector(discoModeChanged:) name:@"discoModeChanged" object:nil];
    [_nc addObserver:self selector:@selector(discoSpeedChanged:) name:@"discoSpeedChanged" object:nil];
    [_nc addObserver:self selector:@selector(discoMIDIChannelChanged:) name:@"discoMIDIChannelChanged" object:nil];
}


- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}

- (void)discoBrightnessChanged:(NSNotification *)n {
    dispatch_queue_t dq = dispatch_get_main_queue( );
    dispatch_async(dq, ^{
        self->_discoBrightnessTextField.stringValue = [NSString stringWithFormat:@"%d", [[n object] intValue]];
    });
}

- (void)discoModeChanged:(NSNotification *)n {
    dispatch_queue_t dq = dispatch_get_main_queue( );
    dispatch_async(dq, ^{
        self->_discoModeTextField.stringValue = [NSString stringWithFormat:@"%d", [[n object] intValue]];
    });
}

- (void)discoSpeedChanged:(NSNotification *)n {
    dispatch_queue_t dq = dispatch_get_main_queue( );
    dispatch_async(dq, ^{
        self->_discoSpeedTextField.stringValue = [NSString stringWithFormat:@"%d", [[n object] intValue]];
    });
}

- (void)discoMIDIChannelChanged:(NSNotification *)n {
    dispatch_queue_t dq = dispatch_get_main_queue( );
    dispatch_async(dq, ^{
        self->_discoMIDIChannelTextField.stringValue = [NSString stringWithFormat:@"%d", [[n object] intValue]];
    });
}

@end
