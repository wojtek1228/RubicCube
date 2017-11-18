//
//  colorsalgorithm.cpp
//  RubicQube
//
//  Created by Wojciech on 18.11.2017.
//  Copyright © 2017 Wojciech. All rights reserved.
//

#include "colorsalgorithm.h"


vector<Scalar>colors_tab[6];
vector<Scalar>pom_colors_container;
vector<Scalar>groups_of_colors[6];
Mat cube_result[6];


void init()
{
    for(int i = 0; i<6; i++)
    {
        Mat m(3,3, CV_32SC1,int(0));
        cube_result[i]=m;
    }
}

void calculateAverageColors(vector<Rect>rectangulars_tab[],Mat imgs[])
{
    Mat image;
    Scalar color;
    for(int i = 0; i<6; i++)
    {
        for(int j = 0; j< rectangulars_tab[i].size(); j++)
        {
            image = imgs[i](rectangulars_tab[i][j]);
            color = mean(image);
            color[3] = i*9+j;
            colors_tab[i].push_back(color);
            pom_colors_container.push_back(color);
        }
        
    }
}

double calc_dist(Scalar point1, Scalar point2)
{
    return sqrt((point1[0]-point2[0])*(point1[0]-point2[0]) + (point1[2]-point2[2])*(point1[2]-point2[2]));
}


void writeToResultTable(Scalar point,int group_num)
{
    int side_number = point[3]/9;
    int elem_number = int(point[3])%9;
    cube_result[side_number].at<int>(elem_number/3,elem_number%3) = group_num;
    
}

void groupColors()
{
    int dist;
    int min_dist;
    int min_index;
    int numberOfsquares;
    int middlesquare = 4;
    Scalar point[6];
    Scalar point1;
    for(int i = 0; i<6; i++)
    {
        point[i] = pom_colors_container[middlesquare];
        pom_colors_container.erase(pom_colors_container.begin()+middlesquare);
        middlesquare += 8;
    }
    
    for (int group_index = 0; group_index<6; group_index++)
    {
        point1 = point[group_index];
        writeToResultTable(point1,group_index);
        numberOfsquares = 1;
        
        while(numberOfsquares<9)
        {
            min_dist = calc_dist(point1, pom_colors_container[0]);
            min_index = 0;
            for(int i = 1; i < pom_colors_container.size(); i++)
            {
                dist = calc_dist(point1, pom_colors_container[i]);
                if(dist<= min_dist)
                {
                    min_dist = dist;
                    min_index = i;
                }
            }
            writeToResultTable(pom_colors_container[min_index],group_index);
            numberOfsquares++;
            pom_colors_container.erase(pom_colors_container.begin()+min_index);
        }
        
    }
    
    
}

void projectPointsOnPlane()
{
    pom_colors_container.clear();
    Scalar color;
    double r_x[3][3] = {{1, 0, 0},{0, cos(3.14/4), -sin(3.14/4)},{0, sin(3.14/4), cos(3.14/4)}};
    double r_z[3][3] = {{cos(-3.14/4), -sin(-3.14/4), 0},{sin(-3.14/4), cos(-3.14/4), 0},{0, 0, 1}};
    Mat rotX = Mat(3, 3, CV_64F, r_x);
    Mat rotZ = Mat(3, 3, CV_64F, r_z);
    for(int i = 0; i<6; i++)
    {
        for(int j = 0; j< 9; j++)
        {
            color = colors_tab[i][j];
            double m[1][3] = {{color[2], color[1], color[0]}};
            Mat color_mat = Mat(1, 3, CV_64F, m);
            color_mat = color_mat*rotZ*rotX;
            color[0] = color_mat.at<double>(0,0);
            color[1] = color_mat.at<double>(0,1);
            color[2] = color_mat.at<double>(0,2);
            pom_colors_container.push_back(color);
            
            
        }
    }
    
    
    
    
}


void runAlgorithm(vector<Rect>rectangulars_tab[],Mat imgs[])
{
    init();
    calculateAverageColors(rectangulars_tab,imgs);
  //saveColors(pom_colors_container,"przed1");   // kontrola kolorów
    projectPointsOnPlane();
  //saveColors(pom_colors_container,"po1");      // kontrola kolorów po transformacji
   
    groupColors();
    showASCIIResult();
}
