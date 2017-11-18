//
//  debugservice.hpp
//  RubicQube
//
//  Created by Wojciech on 18.11.2017.
//  Copyright © 2017 Wojciech. All rights reserved.
//

#ifndef debugservice_h
#define debugservice_h

#include <stdio.h>
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <stdlib.h>
#include "colorsalgorithm.h"


using namespace cv;
using namespace std;

void showGraphicResult(int side_index);
void showASCIIResult();
void drawRect(int k,vector<Rect>rectangulars_tab[],Mat imgs[]);
#endif /* debugservice_h */
