#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "search.h"
#include "solve.h"



using namespace cv;
using namespace std;

Point point1, point2;
int drag = 0;
Rect rect;
Mat img,img2;
Mat res_img = Mat(Size(300,300),CV_8UC3);
int select_flag;
int state=0;
vector<Rect>rectangulars_tab[6];
vector<Scalar>colors_tab[6];
vector<Scalar>pom_colors_container;
vector<Scalar>groups_of_colors[6];
Mat imgs[6];
Mat res[6];
int side_index = 0;
char colorToLetterTab[6];
string solverInput;
string movesForRobot;
Mat cube_result[6];
Mat m(3,3, CV_32SC1);
vector<int>programSequence;
int digOUT[] = {15,16,1,4,5,27,28,29};   // 8 10 12 16 18 36 38 40
int digIN[] = {0,2,3,21,22,23,24,25}; // 11 13 15 29 31 33 35 37

void init()
{
    for(int i = 0; i<6; i++)
    {
        Mat m(3,3, CV_32SC1);
        cube_result[i]=m;
    }
}

/*
void initGPIO()
{
    wiringPiSetup();
    
    for(int i = 0; i<8; i++)
    {
        pinMode(digIN[i],INPUT);
        pinMode(digOUT[i],OUTPUT);
        digitalWrite(digOUT[i],LOW);
    }
    
}
*/

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
        imgs[side_index] = img2;
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
    if(side_number*9+elem_number != point[3]) cout<<"ERROR"<<endl;
    cube_result[side_number].at<int>(elem_number/3,elem_number%3) = group_num;
    
    
   // cube_result[side_number][elem_number] = group_num;
    
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

void showResult()
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

}

void generateSolverInput()
{
    colorToLetterTab[cube_result[0].at<int>(1,1)] = 'L';
    colorToLetterTab[cube_result[1].at<int>(1,1)] = 'D';
    colorToLetterTab[cube_result[2].at<int>(1,1)] = 'R';
    colorToLetterTab[cube_result[3].at<int>(1,1)] = 'B';
    colorToLetterTab[cube_result[4].at<int>(1,1)] = 'U';
    colorToLetterTab[cube_result[5].at<int>(1,1)] = 'F';

    
    solverInput = "";
   
    //U
    for(int j = 0; j<3; j++)
        for(int k = 0; k<3; k++)
        {
            solverInput += colorToLetterTab[cube_result[4].at<int>(j,k)];
        }
    
    //R
    for(int j = 0; j<3; j++)
        for(int k = 0; k<3; k++)
        {
            solverInput += colorToLetterTab[cube_result[2].at<int>(j,k)];
        }
    
    //F
    for(int j = 0; j<3; j++)
        for(int k = 0; k<3; k++)
        {
            solverInput += colorToLetterTab[cube_result[5].at<int>(j,k)];
        }
    
    //D
    for(int j = 0; j<3; j++)
        for(int k = 0; k<3; k++)
        {
            solverInput += colorToLetterTab[cube_result[1].at<int>(j,k)];
        }
    
    //L
    for(int j = 0; j<3; j++)
        for(int k = 0; k<3; k++)
        {
            solverInput += colorToLetterTab[cube_result[0].at<int>(j,k)];
        }
    
    //B
    for(int j = 0; j<3; j++)
        for(int k = 0; k<3; k++)
        {
            solverInput += colorToLetterTab[cube_result[3].at<int>(j,k)];
        }
    
    
}

void rotateCubeResults()
{
    Mat pom1;
    rotate(cube_result[1], pom1, ROTATE_90_COUNTERCLOCKWISE);
    cube_result[1] = pom1;  //D
    
    Mat pom2;
    rotate(cube_result[3], pom2, ROTATE_180);
    cube_result[3] = pom2;   //B
    
    Mat pom3;
    rotate(cube_result[5], pom3, ROTATE_180);
    cube_result[5] = pom3;   //F
}





void analyseCube()
{
    calculateAverageColors();
    //saveData("przed1");
    projectPointsOnPlane();
    //saveData("po1");
    groupColors();
    //showResult();
    //cout<<"xxxxxxxxxxxxxxxxxxxx"<<endl;
    rotateCubeResults();
    showResult();
    generateSolverInput();
    cout<<solverInput<<endl;
    state = 5;
}

void solveCube()
{
    char pom[64];
    char *facelets = pom;
    strcpy(facelets, solverInput.c_str());
    char *sol = solution(
                         facelets,
                         24,
                         1000,
                         0,
                         "cache"
                         );
    if (sol == NULL) {
        puts("Unsolvable cube!");
    }
    else
    {
        movesForRobot = translateForRobot(sol);
        cout<<movesForRobot;
        free(sol);
    }
    state = 6;
}

void translateMovesToPins()
{

    
    for(int i = 0; i<movesForRobot.size(); i++)
    {
        string temp ="";
        while(movesForRobot[i] != ' ')
        {
            temp +=movesForRobot[i];
            i++;
        }
        
        if(temp == "X") programSequence.push_back(1);
        else if(temp == "x") programSequence.push_back(2);
        else if(temp == "Y") programSequence.push_back(3);
        else if(temp == "y") programSequence.push_back(4);
        else if(temp == "U") programSequence.push_back(5);
        else if(temp == "U2") programSequence.push_back(6);
        else if(temp == "U'") programSequence.push_back(7);
        else if(temp == "D") programSequence.push_back(8);
        else if(temp == "D2") programSequence.push_back(9);
        else if(temp == "D'") programSequence.push_back(10);
    }
}

/*
void sendProgramNumberToRobot(int number)
{
    string binary = std::bitset<4>(number).to_string();
    
    for(int i = 0; i<4; i++)
    {
        if(binary[i]=='1') digitalWrite(digOUT[i],HIGH);
          else
              digitalWrite(digOUT[i],LOW);
    }
    
}

*/


int main()
{
    
    init();
  //  initGPIO();
    VideoCapture cap = VideoCapture(0); /* Start webcam */
    cap >> img; /* get image(Mat) */
    imshow("image", img);
    cout<<"Podaj stan"<<endl;
    cin>>state;
    setMouseCallback("image", mouseHandler, NULL);
    
    while(state!=6)
    {
        switch(state)
        {
            case 1: {camera(cap); break;}
            case 2: {calibration(); break;}
            case 3: {testMode(); break;}
            case 4: {analyseCube();break;}
            case 5: {solveCube();break;}
                
        }
    }
    translateMovesToPins();
    return 0;
}
