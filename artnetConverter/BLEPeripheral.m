//
//  BLEPeripheral.m
//  treeControlApp
//
//  Created by Richard Arima on 6/25/19.
//  Copyright © 2019 Richard Arima. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "BLEPeripheral.h"

@import CoreBluetooth;

@implementation BLEPeripheral

- (id)init
{
    self = [super init];
    if(!self) return self;
    
    mode =  nil;
    brightness = nil;
    speed = nil;
    
    nc = [NSNotificationCenter defaultCenter];
    
    myCentralManager = [[CBCentralManager alloc] initWithDelegate:(id)self queue:nil options:nil];
    
    return self;
}

- (void)centralManager:(CBCentralManager *)central didDiscoverPeripheral:(CBPeripheral *)peripheral
     advertisementData:(NSDictionary *)advertisementData RSSI:(NSNumber *)RSSI
{
    
    NSLog(@"Discovered %@, %@", peripheral.name, peripheral.description);
    if(([peripheral.name isEqualToString:@"treeServ"]) || ([peripheral.name isEqualToString:@"ARDUINO 101-33E0"])) {
        NSLog(@"Found treeServ");
        [myCentralManager stopScan];
        self->currentPeripheral = peripheral;
        [self->myCentralManager connectPeripheral:self->currentPeripheral options:nil];
        /*
         while(Current_Peripheral.state != CBPeripheralStateConnected) {
         [myCentralManager connectPeripheral:Current_Peripheral options:nil];
         sleep(10);
         }
         */
    }
    //self.discoveredPeripheral = peripheral;
}

- (void)centralManager:(CBCentralManager *)central didConnectPeripheral:(nonnull CBPeripheral *)peripheral
{
    NSLog(@"Entered didConnectPeripheral");
    
    mode = nil;
    brightness = nil;
    speed = nil;
    
    [currentPeripheral setDelegate:(id)self]; // THIS IS A QUESTIONABLE STATEMENT AND SHOULD BE LOOKED AT FURTHER
    [currentPeripheral discoverServices: @[[CBUUID UUIDWithString:@"DDDD"]]];
    //[Current_Peripheral discoverServices:nil];
}

- (void)peripheral:(CBPeripheral *)peripheral didDiscoverServices:(NSError *)error
{
    for(CBService *service in peripheral.services) {
        NSLog(@"Discovered service: %@", service);
        [peripheral discoverCharacteristics:nil forService:service];
    }
}

- (void)peripheral:(CBPeripheral *)peripheral didDiscoverCharacteristicsForService:(nonnull CBService *)service error:(nullable NSError *)error
{
    for(CBCharacteristic *characteristic in service.characteristics) {
        NSLog(@"Discovered characteristic: %@", characteristic);
        if([characteristic.UUID isEqual:[CBUUID UUIDWithString:@"0050"]]) {
            [peripheral setNotifyValue: YES forCharacteristic:characteristic];
            mode = characteristic;
            
            //            NSData *updatedValue = characteristic.value;
            //            int32_t *intDataPointer = (int32_t *)[updatedValue bytes];
            //            if(!intDataPointer) {
            //                NSLog(@"Error in didUpdateValueForCharacteristic in [updatedValue bytes]");
            //                return;
            //            }
        }
        else if([characteristic.UUID isEqual:[CBUUID UUIDWithString:@"0051"]]) {
            [peripheral setNotifyValue: YES forCharacteristic:characteristic];
            brightness = characteristic;
        }
        else if([characteristic.UUID isEqual:[CBUUID UUIDWithString:@"0052"]]) {
            [peripheral setNotifyValue: YES forCharacteristic:characteristic];
            speed = characteristic;
        }
        else {
            NSLog(@"Unknown characteristic discovered: %@", characteristic);
        }
    }
    
    //    if(mode && brightness && speed) {
    //        [nc postNotificationName:@"BLEConnected" object:nil];
    //    }
}

- (void)peripheral:(CBPeripheral *)peripheral didUpdateValueForCharacteristic:(nonnull CBCharacteristic *)characteristic error:(nullable NSError *)error
{
    //    NSData *updatedValue = characteristic.value;
    //    float_t *dataPointer = (float_t *)[updatedValue bytes];
    //    if(!dataPointer) {
    //        NSLog(@"Error in didUpdateValueForCharacteristic in [updatedValue bytes]");
    //        return;
    //    }
    //    int32_t *intDataPointer = (int32_t *)dataPointer;
    //    float_t value = dataPointer[0];
    
    if([characteristic.UUID isEqual:[CBUUID UUIDWithString:@"0050"]]) {
        //NSLog(@"AX: %f", value);
        //NSLog(@"RPY: %d %d %d", (int16_t)dataPointer[0], (int16_t)dataPointer[1], (int16_t)(++dataPointer)[1]);
        
        //[midiConnector pitchBend:(int)value * 40 toChannel:0];
    }
    else if([characteristic.UUID isEqual:[CBUUID UUIDWithString:@"0051"]]) {
        //NSLog(@"AY: %f", value);
    }
    else if([characteristic.UUID isEqual:[CBUUID UUIDWithString:@"0052"]]) {
        //NSLog(@"AY: %f", value);
    }
    else {
        NSLog(@"Unknown characteristic value handled: %@", characteristic);
    }
}

- (void)centralManagerDidUpdateState:(CBCentralManager *)central
{
    NSLog(@"centralManagerDidUpdateState entered");
    
    switch(central.state) {
            //case CBCentralManagerStatePoweredOn:
        case CBManagerStatePoweredOn:
            [myCentralManager scanForPeripheralsWithServices:nil options:nil];
            break;
        case CBManagerStatePoweredOff:
            break;
        case CBManagerStateResetting:
        case CBManagerStateUnsupported:
        case CBManagerStateUnauthorized:
        case CBManagerStateUnknown:
            break;
            
    }
}

- (void)centralManager:(CBCentralManager *)central didDisconnectPeripheral:(CBPeripheral *)peripheral error:(NSError *)error {
    NSLog(@"didDisconnectPeripheral");
    currentPeripheral = nil;
    mode = nil;
    brightness = nil;
    speed = nil;
    [myCentralManager scanForPeripheralsWithServices:nil options:nil];
}

- (void)centralManager:(CBCentralManager *)central didFailToConnectPeripheral:(CBPeripheral *)peripheral error:(NSError *)error {
    NSLog(@"failed to connect");
}

- (void)shutDown
{
    NSLog(@"shutDown called");
    if(myCentralManager) {
        [myCentralManager stopScan];
        if(currentPeripheral) {
            [myCentralManager cancelPeripheralConnection:currentPeripheral];
        }
    }
}

- (void)peripheral:(CBPeripheral *)peripheral didWriteValueForCharacteristic:(CBCharacteristic *)characteristic error:(NSError *)error
{
    if(error) NSLog(@"BLE Write Error: %@", [error localizedDescription]);
}

//- (void)flexUpdated:(NSNotification *)notification
//{
//    GMSGFlex *flex = [[notification userInfo] valueForKey:@"Flex"];
//    if((Current_Peripheral.state == CBPeripheralStateConnected) && flexThumbMiddleChar) {
//        Float32 f[2] = {flex->thumb, flex->middle};
//        NSData *Data = [NSData dataWithBytes:&f length:sizeof(Float32) * 2];
//        [Current_Peripheral writeValue:Data forCharacteristic:flexThumbMiddleChar type:CBCharacteristicWriteWithResponse];
//    }
//}

- (void)writeMode:(NSNumber *)newMode {
    if(currentPeripheral && mode) {
        if(currentPeripheral.state == CBPeripheralStateConnected) {
            int32_t m = [newMode intValue];
            [currentPeripheral writeValue:[NSData dataWithBytes:&m length:sizeof(int32_t)] forCharacteristic:mode type:CBCharacteristicWriteWithResponse];
        }
    }
}

- (void)writeBrightness:(NSNumber *)newBrightness {
    if(currentPeripheral && brightness) {
        if(currentPeripheral.state == CBPeripheralStateConnected) {
            int32_t m = [newBrightness intValue];
            [currentPeripheral writeValue:[NSData dataWithBytes:&m length:sizeof(int32_t)] forCharacteristic:brightness type:CBCharacteristicWriteWithResponse];
        }
    }
}

- (void)writeSpeed:(NSNumber *)newSpeed {
    if(currentPeripheral && speed) {
        if(currentPeripheral.state == CBPeripheralStateConnected) {
            int32_t m = [newSpeed intValue];
            [currentPeripheral writeValue:[NSData dataWithBytes:&m length:sizeof(int32_t)] forCharacteristic:speed type:CBCharacteristicWriteWithResponse];
        }
    }
}

@end


