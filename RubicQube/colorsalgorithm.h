//
//  colorsalgorithm.h
//  RubicQube
//
//  Created by Wojciech on 18.11.2017.
//  Copyright © 2017 Wojciech. All rights reserved.
//

#ifndef colorsalgorithm_h
#define colorsalgorithm_h

#include <stdio.h>
#include "opencv2/imgproc.hpp"
#include "debugservice.h"

using namespace std;
using namespace cv;

extern vector<Scalar>colors_tab[6];
extern vector<Scalar>pom_colors_container;
extern Mat cube_result[6];

void init();
void projectPointsOnPlane();
void calculateAverageColors(vector<Rect>rectangulars_tab[],Mat imgs[]);
double calc_dist(Scalar point1, Scalar point2);
void writeToResultTable(Scalar point,int group_num);
void groupColors();
void projectPointsOnPlane();
void runAlgorithm(vector<Rect>rectangulars_tab[],Mat imgs[]);

#endif /* colorsalgorithm_h */
