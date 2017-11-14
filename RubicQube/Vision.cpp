//
//  Vision.cpp
//  RubicQube
//
//  Created by Wojciech on 21.10.2017.
//  Copyright © 2017 Wojciech. All rights reserved.
//

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

class Vision
{
private:
    Point point1, point2; /* vertical points of the bounding box */
    int drag = 0;
    Rect rect; /* bounding box */
    Mat img, roiImg,img2; /* roiImg - the part of the image in the bounding box */
    Mat res_img = Mat(Size(300,300),CV_8UC3);
    int select_flag;
    int state=0;
    vector<Rect>rectangulars;
    vector<Scalar>colors;
    VideoCapture cap;

   
public:
    Vision()
    {
        init();
    }
    
    void mouseHandler(int event, int x, int y)
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
            roiImg = img(rect);
            if(rectangulars.size()==9) rectangulars.clear();
            rectangulars.push_back(rect);
        }
        
        if (event == CV_EVENT_LBUTTONUP)
        {
            /* ROI selected */
            select_flag = 1;
            drag = 0;
        }
    }
    
    static void onMouse(int event, int x, int y, int, void* userdata)
    {
        Vision* vision = reinterpret_cast<Vision*>(userdata);
        vision->mouseHandler(event, x, y);
    }
    
public:
    void camera()
    {
        
        cap>>img;
        
        img2 = img.clone();
        imshow("image", img);
        
        int k = (waitKey(10) & 0xFF);
        if (k=='n')
        {
            state  = 2;
        }
       
    }
    
    void calibration()
    {
        
        //rectangle(img2, rect, CV_RGB(255, 0, 0), 3, 8, 0);
        //imshow("image", img2);
        int k = (waitKey(10) & 0xFF);
        if (k=='n')
        {
            state  = 3;
        }
        
        
    }
    
    void showAreas()
    {
        for(int i = 0; i<rectangulars.size(); i++)
        {
            cout<<rectangulars[i]<<endl;
        }
        
        state = 4;
    }
    
    void analyse_rectangulars()
    {
        
        Mat image,image_hsv;
        Scalar color;
        string color_name;
        colors.clear();
        
        
        for(int i = 0; i<rectangulars.size(); i++)
        {
            if(i>0) cout<<color_name<<"  "<<color<<"   ";
            if(i%3==0) cout<<endl;
            
            image = img(rectangulars[i]);
            //cvtColor(image,image_hsv,COLOR_BGR2HSV);
            color = mean(image);
            
            if(color[0]>160 && color[1]>160   && color[2]>160) {color_name = "w";colors.push_back(CV_RGB(255, 255, 255));continue;}
            if(color[2]>200 && color [1]>200) {color_name = "y";colors.push_back(CV_RGB(255, 255, 0)); continue;}
            if(color[2]>200 && color [1]>150) {color_name = "o"; colors.push_back(CV_RGB(255, 165, 0)); continue;}
            if(color[2]>100) {color_name = "r";colors.push_back(CV_RGB(255, 0, 0));continue;}
            if(color[1]>100) {color_name = "g";colors.push_back(CV_RGB(0, 255, 0)); continue;}
            if(color[0]>100) {color_name = "b";colors.push_back(CV_RGB(0, 0, 255)); continue;}
            //if((color[0]>0 && color [0]<9) || (color[0]>151 && color [0]<180)) {color_name = "r";colors.push_back(CV_RGB(255, 0, 0));continue;}
            //if(color[1]>0 && color[1]<20 && color[2]>230 && color[2]<255) color_name = "w";{colors.push_back(CV_RGB(255, 255, 255));continue;}
            colors.push_back(Scalar(130,130,130));
            
        }
        cout<<color_name<<"  "<<color<<"   "<<endl;
        
        state = 4;
        
        
    }
    void show_result()
    {

        
        for(int i= 0; i<3; i++)
        {
            Rect rect = Rect(i*100,0,100,100);
            //rectangle(res_img, rect, colors[i],CV_FILLED);
            rectangulars.push_back(rect);
            saveAreasToFile();
        }
        for(int i= 0; i<3; i++)
        {
            Rect rect = Rect(i*100,100,100,100);
            rectangle(res_img, rect, colors[i+3],CV_FILLED);
        }
        
        for(int i= 0; i<3; i++)
        {
            Rect rect = Rect(i*100,200,100,100);
            rectangle(res_img, rect, colors[i+6],CV_FILLED);
        }
        
        //cout<<colors.size();
        imshow("image3",res_img);
        int k = (waitKey(10) & 0xFF);
        if(k == 'n')
        {
            state = 1;
        }
        
                rectangulars.clear();
    }
    
    void saveAreasToFile()
    {
        ofstream myfile;
        myfile.open ("example.txt");
        for(int i= 0; i<rectangulars.size(); i++)
        {
            myfile<<rectangulars[i].x<<" "<<rectangulars[i].y<<" "<<rectangulars[i].width<<" "<<rectangulars[i].height<<endl;
        }
        myfile.close();
    }
    
    void readAreasFromFile()
    {
        ifstream myfile;
        myfile.open("example.txt");
        while(!myfile.eof())
        {
            int x,y,width,height;
            myfile>>x;
            myfile>>y;
            myfile>>width;
            myfile>>height;
            
            Rect rect(x,y,width,height);
            rectangulars.push_back(rect);
        }
        for(int i= 0; i<rectangulars.size(); i++)
        {
            cout<<rectangulars[i]<<endl;
        }
        state =2;

    }
    void init()
    {
        cap = VideoCapture(0); /* Start webcam */
        cap >> img; /* get image(Mat) */
        imshow("image", img);
        //cvSetMouseCallback("image", onMouse, NULL);
        
    }
    
    void logic()
    {   state = 1;
        while(state!=5)
        {
            switch(state)
            {
                case 1: {camera(); break;}
                case 2: {calibration(); break;}
                case 3: {analyse_rectangulars(); break;}
                case 4: {show_result();break;}
            }
            cout<<state<<endl;
        }

    }
    

    

};
