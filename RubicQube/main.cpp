#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "solver.h"
#include "fileservice.h"
#include "colorsalgorithm.h"
#include "debugservice.h"


using namespace cv;
using namespace std;


Point point1, point2;
int drag = 0;
Rect rect;
int select_flag;
Mat img,img2;
int state=0;
vector<Rect>rectangulars_tab[6];
Mat imgs[6];
int side_index = 0;
Mat m(3,3, CV_32SC1);


void mouseHandler(int event, int x, int y, int flags, void* param)
{
    if (event == CV_EVENT_LBUTTONDOWN && !drag)
    {
        /* left button clicked. ROI selection begins */
        point1 = Point(x, y);
        drag = 1;
    }
    
    if (event == CV_EVENT_MOUSEMOVE && drag)
    {
        /* mouse dragged. ROI being selected */
        Mat img1 = img.clone();
        point2 = Point(x, y);
        rectangle(img1, point1, point2, CV_RGB(255, 0, 0), 3, 8, 0);
        imshow("image", img1);
        
    }
    
    if (event == CV_EVENT_LBUTTONUP && drag)
    {
        point2 = Point(x, y);
        rect = Rect(point1.x,point1.y,x-point1.x,y-point1.y);
        drag = 0;
        rectangulars_tab[side_index].push_back(rect);
    }
    
    if (event == CV_EVENT_LBUTTONUP)
    {
        /* ROI selected */
        select_flag = 1;
        drag = 0;
    }
}


void camera(VideoCapture cap)
{
    
    cap>>img;
    
    imshow("image", img);
    
    int k = (waitKey(10) & 0xFF);
    if (k=='n')
    {
        img2 = img.clone();
        state  = 2;
    }
    
}

void calibration()
{
    int k = (waitKey(10) & 0xFF);
    if (k=='n')
    {
        saveImage(img2,side_index);
        saveAreasToFile(rectangulars_tab[side_index],side_index);
        imgs[side_index] = img2;
        side_index++;
        if (side_index < 6) state  = 1;
        else state = 4;
    }
    
}

void loadData()
{
    for(int side = 0; side<6; side++)
    {
        imgs[side] = readImage(side);
        rectangulars_tab[side] = readAreasFromFile(side);

    }
}

void analyseCube()
{
    runAlgorithm(rectangulars_tab, imgs);
    showASCIIResult();
}

void solveCube()
{
    prepareSolverInputAndSolve();
    showASCIIResult();
}

int main()
{
    
    //  initGPIO();
    VideoCapture cap = VideoCapture(0); /* Start webcam */
    cap >> img; /* get image(Mat) */
    setMouseCallback("image", mouseHandler, NULL);
    imshow("image", img);
    
    
    cout<<"Podaj stan"<<endl;
    cin>>state;
    
    while(state!=6)
    {
        switch(state)
        {
            case 1: {camera(cap); break;}
            case 2: {calibration(); break;}
            case 3:
                {
                    loadData();
                    analyseCube();
                    solveCube();
                    state = 6;
                    break;
                }
                
                
        }
    }
    
    for(int i = 0; i<programSequence.size(); i++)
    {
        cout<<programSequence[i]<<endl;

    }
            cout<<programSequence.size();
    return 0;
}
