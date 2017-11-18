//
//  GPIOservice.cpp
//  RubicQube
//
//  Created by Wojciech on 18.11.2017.
//  Copyright © 2017 Wojciech. All rights reserved.
//

#include "GPIOservice.h"

int digOUT[] = {15,16,1,4,5,27,28,29};   // 8 10 12 16 18 36 38 40
int digIN[] = {0,2,3,21,22,23,24,25}; // 11 13 15 29 31 33 35 37

/*
 void initGPIO()
 {
 wiringPiSetup();
 
 for(int i = 0; i<8; i++)
 {
 pinMode(digIN[i],INPUT);
 pinMode(digOUT[i],OUTPUT);
 digitalWrite(digOUT[i],LOW);
 }
 
 }
 */

/*
 void sendProgramNumberToRobot(int number)
 {
 string binary = std::bitset<4>(number).to_string();
 
 for(int i = 0; i<4; i++)
 {
 if(binary[i]=='1') digitalWrite(digOUT[i],HIGH);
 else
 digitalWrite(digOUT[i],LOW);
 }
 
 }
 
 */

