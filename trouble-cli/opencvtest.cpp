#include "opencvtest.h"

using namespace std;
using namespace cv;

/*Regression test for OpticalFlow*/

void OpenCVTest::OpticalFlow()
{
    // Your input images.
    IplImage* A = cvCreateImage(cvSize(100,100),8,1);
    IplImage* B = cvCreateImage(cvSize(100,100),8,1);

    // Size of the grid for feature points A, explained a little more later.
    int gridSize = 4;

    // Number of points to track.
    int count = gridSize*gridSize;

    // C-code. In case of c++ use new/delete instead of malloc/free
    CvPoint2D32f * featuresA = (CvPoint2D32f*) malloc(count*sizeof(CvPoint2D32f));
    CvPoint2D32f * featuresB = (CvPoint2D32f*) malloc(count*sizeof(CvPoint2D32f));

    // Size of the search window
    CvSize winSize = cvSize(2,2);

    // Number of levels in the pyramid
    int level = 2;

    // Status and error
    char * status = (char*) malloc(count*sizeof(char));
    float * err   = (float*) malloc(count*sizeof(float));

    // Set the termination criteria.
    int type = CV_TERMCRIT_ITER|CV_TERMCRIT_EPS;
    double eps = 0.01;
    int iter = 10;
    CvTermCriteria crit = cvTermCriteria(type,iter,eps);

    // Set additional flags, such as CV_LKFLOW_PYR_A_READY,CV_LKFLOW_PYR_B_READY
    // or CV_LKFLOW_INITIAL_GUESS, here I choice not to use any of them.
    int internal_flags = 0;

    // Now we have all variables, just one thing missing..
    // We need some good features to track, ie put something in featuresA.
    // here you can use the cvGoodFeaturesToTrack function, but
    // I will instead just use a simple grid.
    float spacing[2]= {float(A->width)/float(gridSize), float(A->height)/float(gridSize)};
    float x; float y;
    for (int i = 0; i < count; i++){
            x = (i-gridSize*(i/gridSize))*spacing[0]+spacing[0]/2;
            y = (i/gridSize)*spacing[1]+spacing[1]/2;
            featuresA[i] = cvPoint2D32f(x,y);
    }

    // Track the points
    //Calculates an optical flow for a sparse feature set using the iterative Lucas-Kanade method with pyramids.
    cvCalcOpticalFlowPyrLK(A,B,NULL,NULL,featuresA,featuresB,count,winSize,level,status,err,crit,internal_flags);

    // Clean up
    cvReleaseImage(&A);
    cvReleaseImage(&B);
    free(featuresA);
    free(featuresB);
    free(status);
    free(err);

}

int OpenCVTest::OpticalFlowDemo(){
       // Initialize, load two images from the file system, and
       // allocate the images and other structures we will need for
       // results.
            //
            Mat imgA = imread("/home/frank/Desktop/Research/OpenCV/data/OpticalFlow0.jpg",CV_LOAD_IMAGE_GRAYSCALE);
            Mat imgB = imread("/home/frank/Desktop/Research/OpenCV/data/OpticalFlow1.jpg",CV_LOAD_IMAGE_GRAYSCALE);
            Size img_sz = imgA.size();
            int win_size = 10;
            Mat imgC = imread("/home/frank/Desktop/Research/OpenCV/data/OpticalFlow1.jpg",CV_LOAD_IMAGE_UNCHANGED);

            // The first thing we need to do is get the features
            // we want to track.
            //
        vector<Point2f> cornersA, cornersB;
        const int MAX_CORNERS = 500;
            goodFeaturesToTrack(imgA, cornersA, MAX_CORNERS, 0.01, 5, noArray(), 3, false, 0.04);

            cornerSubPix(imgA, cornersA, Size(win_size, win_size), Size(-1,-1),
                     TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03));

            // Call the Lucas Kanade algorithm
            //
        vector<uchar> features_found;
        calcOpticalFlowPyrLK(imgA, imgB, cornersA, cornersB, features_found, noArray(),
                             Size(win_size*4+1,win_size*4+1), 5,
                             TermCriteria( CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, .3 ));
        // Now make some image of what we are looking at:
        //
        for( int i = 0; i < (int)cornersA.size(); i++ ) {
         if( !features_found[i] )
             continue;
         line(imgC, cornersA[i], cornersB[i], Scalar(0,255,0), 2, CV_AA);
      }
      //imshow("ImageA",imgA);
      //imshow("ImageB",imgB);
      //imshow("LKpyr_OpticalFlow",imgC);
      return 0;

}
