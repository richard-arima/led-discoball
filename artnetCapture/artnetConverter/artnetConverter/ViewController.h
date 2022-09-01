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
@property (weak) IBOutlet NSTextField *discoMasterDimmerTextField;
@property (weak) IBOutlet NSTextField *discoDimmerModeTextField;

@property (weak) IBOutlet NSTextField *disco1DTextField;
@property (weak) IBOutlet NSTextField *disco1RTextField;
@property (weak) IBOutlet NSTextField *disco1GTextField;
@property (weak) IBOutlet NSTextField *disco1BTextField;

@property (weak) IBOutlet NSTextField *disco2DTextField;
@property (weak) IBOutlet NSTextField *disco2RTextField;
@property (weak) IBOutlet NSTextField *disco2GTextField;
@property (weak) IBOutlet NSTextField *disco2BTextField;

@property (weak) IBOutlet NSTextField *disco3DTextField;
@property (weak) IBOutlet NSTextField *disco3RTextField;
@property (weak) IBOutlet NSTextField *disco3GTextField;
@property (weak) IBOutlet NSTextField *disco3BTextField;

@property (weak) IBOutlet NSTextField *disco4DTextField;
@property (weak) IBOutlet NSTextField *disco4RTextField;
@property (weak) IBOutlet NSTextField *disco4GTextField;
@property (weak) IBOutlet NSTextField *disco4BTextField;

@property (weak) IBOutlet NSTextField *disco5DTextField;
@property (weak) IBOutlet NSTextField *disco5RTextField;
@property (weak) IBOutlet NSTextField *disco5GTextField;
@property (weak) IBOutlet NSTextField *disco5BTextField;

@property (weak) IBOutlet NSTextField *disco6DTextField;
@property (weak) IBOutlet NSTextField *disco6RTextField;
@property (weak) IBOutlet NSTextField *disco6GTextField;
@property (weak) IBOutlet NSTextField *disco6BTextField;
@property (weak) IBOutlet NSTextField *discoColorModeTextField;

@property (weak) IBOutlet NSTextField *discoADTextField;
@property (weak) IBOutlet NSTextField *discoARTextField;
@property (weak) IBOutlet NSTextField *discoAGTextField;
@property (weak) IBOutlet NSTextField *discoABTextField;
@property (weak) IBOutlet NSTextField *discoAModeTextField;
@property (weak) IBOutlet NSTextField *discoAParameter1TextField;
@property (weak) IBOutlet NSTextField *discoAParameter2TextField;
@property (weak) IBOutlet NSTextField *discoAParameter3TextField;
@property (weak) IBOutlet NSTextField *discoAParameter4TextField;

@property (weak) IBOutlet NSTextField *serialStatusOutputTextField;

@end

