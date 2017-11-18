//
//  debugservice.cpp
//  RubicQube
//
//  Created by Wojciech on 18.11.2017.
//  Copyright © 2017 Wojciech. All rights reserved.
//

#include "debugservice.h"


Mat res_img = Mat(Size(300,300),CV_8UC3);

void showGraphicResult(int side_index)
{
    for(int j = 0; j<6; j++)
    {
        for(int i= 0; i<3; i++)
        {
            Rect rect = Rect(i*100,0,100,100);
            rectangle(res_img, rect, colors_tab[side_index][i],CV_FILLED);
        }
        for(int i= 0; i<3; i++)
        {
            Rect rect = Rect(i*100,100,100,100);
            rectangle(res_img, rect, colors_tab[side_index][i+3],CV_FILLED);
        }
        
        for(int i= 0; i<3; i++)
        {
            Rect rect = Rect(i*100,200,100,100);
            rectangle(res_img, rect, colors_tab[side_index][i+6],CV_FILLED);
        }
    }
    //cout<<colors.size();
    imshow("image3",res_img);
    
}


void showASCIIResult()
{
    for(int i = 0; i<6; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            if(j%3==0) cout<<endl;
            cout<<cube_result[i].at<int>(j/3,j%3)<<"  ";
            
        }
        cout<<endl<<"-------"<<endl;
    }
    
    cout<<"xxxxxxxxxxxxxxxxxxxx"<<endl;
    
}

void drawRect(int k,vector<Rect>rectangulars_tab[],Mat imgs[])
{
    Mat img1 = imgs[k];
    for(int i = 0; i<9; i++)
    {
        int h =rectangulars_tab[k][i].height;
        int w =rectangulars_tab[k][i].width;
        Point point1(rectangulars_tab[k][i].x,rectangulars_tab[k][i].y);
        Point point2(rectangulars_tab[k][i].x+w,rectangulars_tab[k][i].y+h);
        rectangle(img1, point1, point2, CV_RGB(255, 0, 0), 3, 8, 0);
    }
    
    namedWindow( "Display", WINDOW_AUTOSIZE );
    imshow("Display",img1);
    waitKey(0);
}

