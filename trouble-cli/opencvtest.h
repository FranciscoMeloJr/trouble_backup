#ifndef OPENCVTEST_H
#define OPENCVTEST_H

#include "opencv2/opencv.hpp"

#include <iostream>

using namespace std;
using namespace cv;

class OpenCVTest
{
public:
    //Info:
    IplImage* A;
    IplImage* B;

    int level = 2;
    char * status;
    float * err;

    CvSize winSize;
    CvPoint2D32f * featuresA;
    CvPoint2D32f * featuresB;
    CvTermCriteria crit;
    int gridSize = 4;
    int count = gridSize*gridSize;

    Mat imgA;
    Mat imgB;
    Mat imgC;
    Size img_sz;
    int win_size;
    vector<Point2f> cornersA, cornersB;
    vector<uchar> features_found;
    const int MAX_CORNERS = 500;

    int internal_flags = 0;
    //Control Flow:
    void OpticalFlow();
    int OpticalFlowDemo();
    int setUp();
    int cleanUp(bool t);
    int callOpticalFlow();

    //Display test:
    int Display( int argc, char** argv);

    //Houghlines:
    int HoughLines( int argc, char** argv );

    //Facecompare:
    int FaceCompare( int argc, char** argv );
    void detectAndDraw( Mat& img, CascadeClassifier& cascade, CascadeClassifier& nestedCascade, double scale, bool tryflip );

    //Help
    void help();
    ~OpenCVTest();
};

#endif // OPENCVTEST_H
