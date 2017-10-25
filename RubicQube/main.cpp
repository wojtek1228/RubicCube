#include "Vision.cpp"
#include <iostream>

using namespace cv;
using namespace std;

int state;

int main()
{
    Vision *vision = new Vision();
    vision->init();
    cout<<"Podaj stan"<<endl;
    cin>>state;
    vision->logic();
    
    return 0;
}
