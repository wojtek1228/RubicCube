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
vector<Scalar>pom_colors_container;
vector<Scalar>groups_of_colors[6];
Mat imgs[6];
int side_index = 0;
int cube_result[6][9];


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

void saveData(String name)
{
    ofstream myfile;
    string dataFileName = name;
    myfile.open (dataFileName);
    for(int i= 0; i<pom_colors_container.size(); i++)
    {
        Scalar point = pom_colors_container[i];
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
    if(side_index ==6) {state = 4; }
}

void calculateAverageColors()
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
    if(side_number*9+elem_number != point[3]) cout<<"ERROR"<<endl;
    cube_result[side_number][elem_number] = group_num;
}

void groupColors()
{
    int dist;
    int min_dist;
    int min_index;
    Scalar point1, point2;
    for (int group_index = 0; group_index<6; group_index++)
    {
        point1 = pom_colors_container[0];
        groups_of_colors[group_index].push_back(point1);
        writeToResultTable(point1,group_index);
        pom_colors_container.erase(pom_colors_container.begin());
        
        while(groups_of_colors[group_index].size()<9)
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
            groups_of_colors[group_index].push_back(pom_colors_container[min_index]);
            pom_colors_container.erase(pom_colors_container.begin()+min_index);
        }
    }
    

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
            color[0] = color_mat.at<double>(0,0);
            color[1] = color_mat.at<double>(0,1);
            color[2] = color_mat.at<double>(0,2);
            pom_colors_container.push_back(color);

       
        }
    }
    

}

void analyseCube()
{
    
    
    calculateAverageColors();
  // saveData("przed");
    projectPointsOnPlane();
 //saveData("po");
    groupColors();
    for(int i = 0; i<6; i++)
    {
        for(int j = 0; j< 9; j++)
        {
            if(j%3==0) cout<<endl;
            cout<<cube_result[i][j]<<"  ";
            
        }
        cout<<endl<<"-------"<<endl;
    }
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
