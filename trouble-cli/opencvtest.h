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

    //hough
    Mat src;
    Mat dst, cdst;
    vector<Vec4i> linesVec;

    //FaceDetection:
    CascadeClassifier cascade, nestedCascade;
    double scale;
    bool tryflip;

    Mat image;
    int internal_flags = 0;

    //Edge:
    int edgeThresh = 1;
    Mat gray, edge, cedge;

    //Control Flow:
    void OpticalFlow();
    int OpticalFlowDemo();
    int setUp();
    int cleanUp(bool t);
    int callOpticalFlow();

    //Display test:
    int Read(string path, bool flag);
    int DisplayDemo( int argc, char** argv);

    //Houghlines:
    int HoughLinesDemo(string path, bool flag);
    void HoughLines();
    void start(string);
    void cleanHough(bool flag);

    //Facecompare:
    int FaceDetectionDemo();
    int FaceDetection();
    int inicialization();
    void detectAndDraw( Mat& img, CascadeClassifier& cascade, CascadeClassifier& nestedCascade, double scale, bool tryflip );
    void finish();

    //Edge detection:
    void EdgeDemo(string, bool);
    static void onTrackbar(int, void*);

    //Help
    void version();
    ~OpenCVTest();
};

#endif // OPENCVTEST_H
