#include "opencvtest.h"

using namespace std;
using namespace cv;

/*Regression test for OpticalFlow*/

int OpenCVTest::setUp(){

    // Your input images.
    imgA = imread("/home/frank/Desktop/Research/OpenCV/data/OpticalFlow0.jpg",CV_LOAD_IMAGE_GRAYSCALE);
    imgB = imread("/home/frank/Desktop/Research/OpenCV/data/OpticalFlow1.jpg",CV_LOAD_IMAGE_GRAYSCALE);
    img_sz = imgA.size();
    win_size = 10;
    imgC = imread("/home/frank/Desktop/Research/OpenCV/data/OpticalFlow1.jpg",CV_LOAD_IMAGE_UNCHANGED);

    goodFeaturesToTrack(this->imgA, cornersA, MAX_CORNERS, 0.01, 5, noArray(), 3, false, 0.04);

    cornerSubPix(imgA, cornersA, Size(this->win_size, this->win_size), Size(-1,-1),
             TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03));
    return 0;
}

int OpenCVTest::callOpticalFlow(){

      calcOpticalFlowPyrLK(this->imgA, this->imgB, this->cornersA, this->cornersB, this->features_found, noArray(),
                             Size(this->win_size*4+1,this->win_size*4+1), 5,
                             TermCriteria( CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, .3 ));
      return 0;

}

int OpenCVTest::cleanUp(bool show){

    for( int i = 0; i < (int)cornersA.size(); i++ ) {
     if( !features_found[i] )
         continue;
     line(imgC, cornersA[i], cornersB[i], Scalar(0,255,0), 2, CV_AA);
  }
  if(show){
    imshow("ImageA",imgA);
    imshow("ImageB",imgB);
    imshow("LKpyr_OpticalFlow",imgC);
    }
  waitKey();

  return 0;
}
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
    cvCalcOpticalFlowPyrLK(A,B,NULL,NULL,featuresA,featuresB,count,winSize,this->level,status,err,crit,internal_flags);

    // Clean up
    cvReleaseImage(&A);
    cvReleaseImage(&B);
    free(featuresA);
    free(featuresB);
    free(status);
    free(err);

}

int OpenCVTest::Read(string path , bool show ){

    image = imread(path, CV_LOAD_IMAGE_COLOR);
    if(show){
        namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
        imshow( "Display window", image );                   // Show our image inside it.

        waitKey(0);                                          // Wait for a keystroke in the window
    }

    return 0;
}
//Display Image
int OpenCVTest::DisplayDemo( int argc, char** argv ){
    if( argc != 2)
        {
         cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
         return -1;
        }

        Mat image;
        image = imread(argv[1], CV_LOAD_IMAGE_COLOR);   // Read the file

        if(! image.data )                              // Check for invalid input
        {
            cout <<  "Could not open or find the image" << std::endl ;
            return -1;
        }

        namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
        imshow( "Display window", image );                   // Show our image inside it.

        waitKey(0);                                          // Wait for a keystroke in the window
        return 0;
}

void OpenCVTest::HoughLines(){

        HoughLinesP(dst, this->linesVec, 1, CV_PI/180, 50, 50, 10 );

}
void OpenCVTest::start(string path){

    src = imread(path, 0);

    Canny(src, dst, 50, 200, 3);
    cvtColor(dst, cdst, COLOR_GRAY2BGR);

}
void OpenCVTest::cleanHough(bool flag){

        for( size_t i = 0; i < this->linesVec.size(); i++ )
        {
            Vec4i l = this->linesVec[i];
            line( this->cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, LINE_AA);
        }
        if(flag){
            imshow("source", this->src);
            imshow("detected lines", this->cdst);
            waitKey();
        }
}

int OpenCVTest::HoughLinesDemo(string path, bool flag){

        Mat src = imread(path, 0);

        Mat dst, cdst;
        Canny(src, dst, 50, 200, 3);
        cvtColor(dst, cdst, COLOR_GRAY2BGR);

    #if 0
        vector<Vec2f> lines;
        HoughLines(dst, lines, 1, CV_PI/180, 100, 0, 0 );
        cout << "xxxx";
        for( size_t i = 0; i < lines.size(); i++ )
        {
            float rho = lines[i][0], theta = lines[i][1];
            Point pt1, pt2;
            double a = cos(theta), b = sin(theta);
            double x0 = a*rho, y0 = b*rho;
            pt1.x = cvRound(x0 + 1000*(-b));
            pt1.y = cvRound(y0 + 1000*(a));
            pt2.x = cvRound(x0 - 1000*(-b));
            pt2.y = cvRound(y0 - 1000*(a));
            line( cdst, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
        }
    #else
        cout << "yyy";
        vector<Vec4i> lines;
        HoughLinesP(dst, lines, 1, CV_PI/180, 50, 50, 10 );
        for( size_t i = 0; i < lines.size(); i++ )
        {
            Vec4i l = lines[i];
            line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, LINE_AA);
        }
    #endif
        if(flag){
            imshow("source", src);
            imshow("detected lines", cdst);
            waitKey();
        }

        return 0;

}

//FaceCompare
int OpenCVTest::inicialization(){
    string inputName;
    string path = "/home/frank/Desktop/Research/OpenCV/data/obama.jpg";
    string cascadeName= "data/haarcascades/haarcascade_frontalface_alt.xml";
    string nestedCascadeName = "data/haarcascades/haarcascade_eye_tree_eyeglasses.xml";

    if ( !this->nestedCascade.load( nestedCascadeName ) )
            cerr << "WARNING: Could not load classifier cascade for nested objects" << endl;
    if ( !this->cascade.load( cascadeName ) )
            cerr << "WARNING: Could not load classifier cascade for nested objects" << endl;


    this->scale = 1;
    tryflip = false;
    inputName = path;
    cout << "1";
    image = imread( "data/obama.jpg", 1 );
    if(image.empty()) cout << "Couldn't read image" << endl;
}
int OpenCVTest::FaceDetection(){
    double t = 0;

    vector<Rect> faces, faces2;
    const static Scalar colors[] =
    {
        Scalar(255,0,0),
        Scalar(255,128,0),
        Scalar(255,255,0),
        Scalar(0,255,0),
        Scalar(0,128,255),
        Scalar(0,255,255),
        Scalar(0,0,255),
        Scalar(255,0,255)
    };
    Mat gray, smallImg;

    cvtColor( image, gray, COLOR_BGR2GRAY );
    double fx = 1 / scale;
    resize( gray, smallImg, Size(), fx, fx, INTER_LINEAR );
    equalizeHist( smallImg, smallImg );

    t = (double)getTickCount();
    cascade.detectMultiScale( smallImg, faces,
        1.1, 2, 0
        //|CASCADE_FIND_BIGGEST_OBJECT
        //|CASCADE_DO_ROUGH_SEARCH
        |CASCADE_SCALE_IMAGE,
        Size(30, 30) );

}

void OpenCVTest::finish(){

}

int OpenCVTest::FaceDetectionDemo(){

        Mat image;
        string inputName;
        bool tryflip;
        CascadeClassifier cascade, nestedCascade;
        double scale;
        string path = "/home/frank/Desktop/Research/OpenCV/data/obama.jpg";
        string cascadeName;
        string nestedCascadeName;
        bool sec = true;
        bool flag = false;


        cascadeName = "data/haarcascades/haarcascade_frontalface_alt.xml";
        nestedCascadeName = "data/haarcascades/haarcascade_eye_tree_eyeglasses.xml";

        if ( !nestedCascade.load( nestedCascadeName ) )
                cerr << "WARNING: Could not load classifier cascade for nested objects" << endl;
        if ( !cascade.load( cascadeName ) )
                cerr << "WARNING: Could not load classifier cascade for nested objects" << endl;


        scale = 1.0;
        tryflip = false;
        inputName = path;
        cout << "1";
        image = imread( "data/obama.jpg", 1 );
        if(image.empty()) cout << "Couldn't read image" << endl;


        detectAndDraw( image, cascade, nestedCascade, scale, tryflip );

        return 0;
    }

void OpenCVTest:: detectAndDraw( Mat& img, CascadeClassifier& cascade, CascadeClassifier& nestedCascade, double scale, bool tryflip ){

        double t = 0;

        vector<Rect> faces, faces2;
        const static Scalar colors[] =
        {
            Scalar(255,0,0),
            Scalar(255,128,0),
            Scalar(255,255,0),
            Scalar(0,255,0),
            Scalar(0,128,255),
            Scalar(0,255,255),
            Scalar(0,0,255),
            Scalar(255,0,255)
        };
        Mat gray, smallImg;

        cvtColor( img, gray, COLOR_BGR2GRAY );
        double fx = 1 / scale;
        resize( gray, smallImg, Size(), fx, fx, INTER_LINEAR );
        equalizeHist( smallImg, smallImg );

        t = (double)getTickCount();
        cascade.detectMultiScale( smallImg, faces,
            1.1, 2, 0
            //|CASCADE_FIND_BIGGEST_OBJECT
            //|CASCADE_DO_ROUGH_SEARCH
            |CASCADE_SCALE_IMAGE,
            Size(30, 30) );
        if( tryflip )
        {
            flip(smallImg, smallImg, 1);
            cascade.detectMultiScale( smallImg, faces2,
                                     1.1, 2, 0
                                     //|CASCADE_FIND_BIGGEST_OBJECT
                                     //|CASCADE_DO_ROUGH_SEARCH
                                     |CASCADE_SCALE_IMAGE,
                                     Size(30, 30) );
            for( vector<Rect>::const_iterator r = faces2.begin(); r != faces2.end(); ++r )
            {
                faces.push_back(Rect(smallImg.cols - r->x - r->width, r->y, r->width, r->height));
            }
        }
        t = (double)getTickCount() - t;
        //printf( "detection time = %g ms\n", t*1000/getTickFrequency());
        for ( size_t i = 0; i < faces.size(); i++ )
        {
            Rect r = faces[i];
            Mat smallImgROI;
            vector<Rect> nestedObjects;
            Point center;
            Scalar color = colors[i%8];
            int radius;

            double aspect_ratio = (double)r.width/r.height;
            if( 0.75 < aspect_ratio && aspect_ratio < 1.3 )
            {
                center.x = cvRound((r.x + r.width*0.5)*scale);
                center.y = cvRound((r.y + r.height*0.5)*scale);
                radius = cvRound((r.width + r.height)*0.25*scale);
                circle( img, center, radius, color, 3, 8, 0 );
            }
            else
                rectangle( img, cvPoint(cvRound(r.x*scale), cvRound(r.y*scale)),
                           cvPoint(cvRound((r.x + r.width-1)*scale), cvRound((r.y + r.height-1)*scale)),
                           color, 3, 8, 0);
            if( nestedCascade.empty() )
                continue;
            smallImgROI = smallImg( r );
            nestedCascade.detectMultiScale( smallImgROI, nestedObjects,
                1.1, 2, 0
                //|CASCADE_FIND_BIGGEST_OBJECT
                //|CASCADE_DO_ROUGH_SEARCH
                //|CASCADE_DO_CANNY_PRUNING
                |CASCADE_SCALE_IMAGE,
                Size(30, 30) );
            for ( size_t j = 0; j < nestedObjects.size(); j++ )
            {
                Rect nr = nestedObjects[j];
                center.x = cvRound((r.x + nr.x + nr.width*0.5)*scale);
                center.y = cvRound((r.y + nr.y + nr.height*0.5)*scale);
                radius = cvRound((nr.width + nr.height)*0.25*scale);
                circle( img, center, radius, color, 3, 8, 0 );
            }
        }
        //imshow( "result", img );

}

//help
void OpenCVTest::version(){

        cout << CV_VERSION << "\n" << endl;

}
