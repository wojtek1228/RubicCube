#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <types.hpp>
#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

Point point1, point2;
int drag = 0;
Rect rect;
Mat img, roiImg,img2;
Mat res_img = Mat(Size(300,300),CV_8UC3);
int select_flag;
int state=0;
vector<Rect>rectangulars_tab[6];
vector<Scalar>colors_tab[6];
Scalar pom_colors_tab;
Mat imgs[6];
int side_index = 0;


void saveAreasToFile()
{
    ofstream myfile;
    string dataFileName = "areas" + to_string(side_index) + ".txt";
    myfile.open (dataFileName);
    for(int i= 0; i<rectangulars_tab[side_index].size(); i++)
    {
        myfile<<rectangulars_tab[side_index][i].x<<" "<<rectangulars_tab[side_index][i].y<<" "<<rectangulars_tab[side_index][i].width<<" "<<rectangulars_tab[side_index][i].height<<endl;
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
        roiImg = img(rect);
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
        state  = 2;
    }
    
}

void saveImage()
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

void calibration()
{
    int k = (waitKey(10) & 0xFF);
    if (k=='n')
    {
        saveImage();
        saveAreasToFile();
        imgs[side_index] = img;
        side_index++;
        if (side_index < 6) state  = 1;
        else state = 4;
    }
    
}



void showAreas()
{
    for(int i = 0; i<rectangulars_tab[side_index].size(); i++)
    {
        cout<<rectangulars_tab[side_index][i]<<endl;
    }
    
    state = 4;
}

void show_result()
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
    
    //cout<<colors.size();
    imshow("image3",res_img);
    int k = (waitKey(10) & 0xFF);
    if(k == 'n')
    {
        state = 1;
    }
    

}

void testMode()
{
    imgs[side_index] = readImage(side_index);
    rectangulars_tab[side_index] = readAreasFromFile(side_index);
    side_index++;
    cout<<"jestem";
    if(side_index ==6) {state = 4; }
}

Scalar mean2(Mat m)
{
    Scalar color;
    int r= 0;
    int g = 0;
    int b = 0;
    for(int i=0; i<m.rows; i++)
    {
        for(int j=0; j<m.cols; j++)
        {
            color = m.at<double>(i,j);
            r = r+color[2];
            b = b+color[1];
            g = g+color[0];
        }
    }
    color[0] = r/(m.rows*m.cols);
    color[1] = g/(m.rows*m.cols);
    color[2] = b/(m.rows*m.cols);
    return color;
}

void calculateAverageColors()
{
    Mat image;
    Scalar color;
     cout<<"byłęm";
    for(int i = 0; i<6; i++)
    {
        for(int j = 0; j< rectangulars_tab[i].size(); j++)
        {
            image = imgs[6](rectangulars_tab[i][j]);
            color = mean(image);
            colors_tab[i].push_back(color);
        }
        
    }
   
}

void groupColors()
{
 
}

void projectPointsOnPlane()
{
   
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
            color[0] = color_mat.at<double>(1,0);
            color[1] = color_mat.at<double>(1,1);
            color[2] = color_mat.at<double>(1,2);
            colors_tab[i][j] = color;
        }
    }
    

}
void analyseCube()
{
    
    
    calculateAverageColors();
    pom_colors_tab = colors_tab[0][0];
    projectPointsOnPlane();
    groupColors();
    
}




int main()
{
    
    
    VideoCapture cap = VideoCapture(0); /* Start webcam */
    cap >> img; /* get image(Mat) */
    imshow("image", img);
    cout<<"Podaj stan"<<endl;
    cin>>state;
    setMouseCallback("image", mouseHandler, NULL);
    
    while(state!=5)
    {
        switch(state)
        {
            case 1: {camera(cap); break;}
            case 2: {calibration(); break;}
            case 3: {testMode(); break;}
            case 4: {analyseCube();break;}
                
        }
        
    }
    cin>>state;
    return 0;
}
