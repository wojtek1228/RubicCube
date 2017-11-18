//
//  fileservice.cpp
//  RubicQube
//
//  Created by Wojciech on 18.11.2017.
//  Copyright © 2017 Wojciech. All rights reserved.
//

#include "fileservice.h"



void saveAreasToFile(vector<Rect>side_rectangulars, int side_index)
{
    ofstream myfile;
    string dataFileName = "areas" + to_string(side_index) + ".txt";
    myfile.open (dataFileName);
    for(int i= 0; i<side_rectangulars.size(); i++)
    {
        myfile<<side_rectangulars[i].x<<" "<<side_rectangulars[i].y<<" "<<side_rectangulars[i].width<<" "<<side_rectangulars[i].height<<endl;
    }
    myfile.close();
}

void saveColors(vector<Scalar>colors_container, string name)
{
    ofstream myfile;
    string dataFileName = name;
    myfile.open (dataFileName);
    for(int i= 0; i<colors_container.size(); i++)
    {
        Scalar point = colors_container[i];
        myfile<<point[0]<<" "<<point[1]<<" "<<point[2]<<endl;
    }
    myfile.close();
    
}




vector<Rect> readAreasFromFile(int index)
{
    vector<Rect> rect_list;
    ifstream myfile;
    string dataFileName = "areas" + to_string(index) + ".txt";
    myfile.open(dataFileName);
    for(int i =0; i<9; i++)
    {
        int x,y,width,height;
        myfile>>x;
        myfile>>y;
        myfile>>width;
        myfile>>height;
        
        Rect rect(x,y,width,height);
        rect_list.push_back(rect);
    }
    
    
    return rect_list;
}


void saveImage(Mat img,int side_index)
{
    string imageFileName = "image" + to_string(side_index) + ".jpg";
    imwrite( imageFileName, img );
}

Mat readImage(int index)
{
    string imageFileName = "image" + to_string(index) + ".jpg";
    Mat image = imread(imageFileName, 1);
    return image;
}
