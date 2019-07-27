//
//  ViewController.h
//  artnetConverter
//
//  Created by Richard Arima on 7/23/19.
//  Copyright © 2019 Richard Arima. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface ViewController : NSViewController

@property (nonatomic, strong) NSNotificationCenter *nc;

@property (weak) IBOutlet NSTextField *discoMIDIChannelTextField;
@property (weak) IBOutlet NSTextField *discoBrightnessTextField;
@property (weak) IBOutlet NSTextField *discoModeTextField;
@property (weak) IBOutlet NSTextField *discoSpeedTextField;

@end

