//
//  solver.h
//  RubicQube
//
//  Created by Wojciech on 14.11.2017.
//  Copyright © 2017 Wojciech. All rights reserved.
//

#ifndef solver_h
#define solver_h

#include <stdio.h>
#include <iostream>
#include "opencv2/imgproc.hpp"
#include "colorsalgorithm.h"
#include "search.h"



using namespace std;
using namespace cv;

extern vector<int>programSequence;


char rotationLookUp(char up, char front, char next);
char oppositeSide(char side);
std::string translateForRobot(std::string sol);
void translateMovesToPins();
void rotateCubeResults();
void generateSolverInput();
void solve();
void prepareSolverInputAndSolve();
#endif /* solver_h */
