#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <types.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// helper function:
// finds a cosine of angle between vectors
// from pt0->pt1 and from pt0->pt2
Point point1, point2; /* vertical points of the bounding box */
int drag = 0;
Rect rect; /* bounding box */
Mat img, roiImg,img2; /* roiImg - the part of the image in the bounding box */
Mat res_img = Mat(Size(300,300),CV_8UC3);
int select_flag;
int state=0;
vector<Rect>rectangulars;
vector<Scalar>colors;


static double angle( Point pt1, Point pt2, Point pt0 )
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
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

/*static void drawSquares( Mat& image, const vector<vector<Point> >& squares,vector<Scalar >& detected_colors  )
{
    detected_colors.clear();
    Mat roi_hsv;
    for( size_t i = 0; i < squares.size(); i++ )
    {
        const Point* p = &squares[i][0];
        int n = (int)squares[i].size();
        int shift = 1;
        
        Rect r=boundingRect( Mat(squares[i]));
        r.x = r.x + r.width / 4;
        r.y = r.y + r.height / 4;
        r.width = r.width / 2;
        r.height = r.height / 2;
        
        Mat roi = image(r);
        Scalar color = mean(roi);
        cvtColor(roi,roi_hsv,COLOR_BGR2HSV);
        Scalar color2 = mean(roi_hsv);
        //BRG
        detected_colors.push_back(color2);
        polylines(image, &p, &n, 1, true, color, 2, LINE_AA, shift);
        
        Point center( r.x + r.width/2, r.y + r.height/2 );
        ellipse( image, center, Size( r.width/2, r.height/2), 0, 0, 360, color, 2, LINE_AA );
    }
}
*/
// returns sequence of squares detected on the image.
// the sequence is stored in the specified memory storage
static void findSquares( const Mat& image, vector<vector<Point> >& squares , bool inv=false)
{
    squares.clear();
    
    Mat gray,gray0;
    
    vector<vector<Point> > contours;
    
    cvtColor(image,gray0,COLOR_BGR2GRAY);
    GaussianBlur(gray0, gray0, Size(7,7), 1.5, 1.5);
    Canny(gray0,gray, 0, 30, 3);
    
    // find contours and store them all as a list
    findContours(gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
    
    vector<Point> approx;
    
    // test each contour
    for( size_t i = 0; i < contours.size(); i++ )
    {
        // approximate contour with accuracy proportional
        // to the contour perimeter
        approxPolyDP(Mat(contours[i]), approx, 9, true);
        
        // square contours should have 4 vertices after approximation
        // relatively large area (to filter out noisy contours)
        // and be convex.
        // Note: absolute value of an area is used because
        // area may be positive or negative - in accordance with the
        // contour orientation
        if( approx.size() == 4 &&
           fabs(contourArea(Mat(approx))) > 5 &&
           isContourConvex(Mat(approx)) )
        {
            double maxCosine = 0;
            
            for( int j = 2; j < 5; j++ )
            {
                // find the maximum cosine of the angle between joint edges
                double cosine = fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
                maxCosine = MAX(maxCosine, cosine);
            }
            
            // if cosines of all angles are small
            // (all angles are ~90 degree) then write quandrange
            // vertices to resultant sequence
            if( maxCosine < 0.3 )
                squares.push_back(approx);
        }
    }
}

void camera(VideoCapture cap)
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
        rectangle(res_img, rect, colors[i],CV_FILLED);
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
            case 3: {analyse_rectangulars(); break;}
            case 4: {show_result();break;}
           
        }
       
    }
    cin>>state;
    return 0;
}
