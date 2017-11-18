//
//  fileservice.h
//  RubicQube
//
//  Created by Wojciech on 18.11.2017.
//  Copyright © 2017 Wojciech. All rights reserved.
//

#ifndef fileservice_h
#define fileservice_h

#include <stdio.h>
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <fstream>
using namespace std;
using namespace cv;

void saveAreasToFile(vector<Rect>side_rectangulars, int side_index);
void saveColors(vector<Scalar>colors_container, string name);
void saveImage(Mat img,int side_index);
Mat readImage(int index);
vector<Rect> readAreasFromFile(int index);

#endif /* fileservice */
