//
//  solve.h
//  RubicQube
//
//  Created by Wojciech on 14.11.2017.
//  Copyright © 2017 Wojciech. All rights reserved.
//

#ifndef solve_h
#define solve_h
char rotationLookUp(char up, char front, char next);
char oppositeSide(char side);
std::string translateForRobot(std::string sol);

#endif /* solve_h */
