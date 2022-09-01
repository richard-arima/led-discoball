//
//  BLEPeripheral.h
//  artnetConverter
//
//  Created by Richard Arima on 7/23/19.
//  Copyright © 2019 Richard Arima. All rights reserved.
//

#ifndef BLEPeripheral_h
#define BLEPeripheral_h

@import CoreBluetooth;

@interface BLEPeripheral : NSObject <CBCentralManagerDelegate> {
    NSNotificationCenter *nc;
    
    CBCentralManager *myCentralManager;
    CBPeripheral *currentPeripheral;
    
    CBCharacteristic *mode, *brightness, *speed;
}

- (id)init;
- (void)shutDown;

- (void)writeMode:(NSNumber *)newMode;
- (void)writeBrightness:(NSNumber *)newBrightness;
- (void)writeSpeed:(NSNumber *)newSpeed;

//-(void)centralManagerDidUpdateState:(CBCentralManager *)central;

@end

#endif /* BLEPeripheral_h */

