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

int OpenCVTest::Display(string path , bool show ){

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

//HoughLines
int OpenCVTest::HoughLines( int argc, char** argv ){
    cv::CommandLineParser parser(argc, argv,
            "{help h||}{@image|../data/pic1.png|}"
        );
        if (parser.has("help"))
        {
            help();
            return 0;
        }
        string filename = parser.get<string>("@image");
        if (filename.empty())
        {
            help();
            cout << "no image_name provided" << endl;
            return -1;
        }
        Mat src = imread(filename, 0);
        if(src.empty())
        {
            help();
            cout << "can not open " << filename << endl;
            return -1;
        }

        Mat dst, cdst;
        Canny(src, dst, 50, 200, 3);
        cvtColor(dst, cdst, COLOR_GRAY2BGR);

    #if 0
        vector<Vec2f> lines;
        HoughLines(dst, lines, 1, CV_PI/180, 100, 0, 0 );
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
        vector<Vec4i> lines;
        HoughLinesP(dst, lines, 1, CV_PI/180, 50, 50, 10 );
        for( size_t i = 0; i < lines.size(); i++ )
        {
            Vec4i l = lines[i];
            line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, LINE_AA);
        }
    #endif
        //imshow("source", src);
        //imshow("detected lines", cdst);

        waitKey();

        return 0;

}

//FaceCompare
int OpenCVTest::FaceCompare( int argc, char** argv ){

        VideoCapture capture;
        Mat frame, image;
        string inputName;
        bool tryflip;
        CascadeClassifier cascade, nestedCascade;
        double scale;

        string cascadeName;
        string nestedCascadeName;

        cv::CommandLineParser parser(argc, argv,
            "{help h||}"
            "{cascade|../data/haarcascades/haarcascade_frontalface_alt.xml|}"
            "{nested-cascade|../data/haarcascades/haarcascade_eye_tree_eyeglasses.xml|}"
            "{scale|1|}{try-flip||}{@filename||}"
        );
        if (parser.has("help"))
        {
            help();
            return 0;
        }
        cascadeName = parser.get<string>("cascade");
        nestedCascadeName = parser.get<string>("nested-cascade");
        scale = parser.get<double>("scale");
        if (scale < 1)
            scale = 1;
        tryflip = parser.has("try-flip");
        inputName = parser.get<string>("@filename");
        if (!parser.check())
        {
            parser.printErrors();
            return 0;
        }
        if ( !nestedCascade.load( nestedCascadeName ) )
            cerr << "WARNING: Could not load classifier cascade for nested objects" << endl;
        if( !cascade.load( cascadeName ) )
        {
            cerr << "ERROR: Could not load classifier cascade" << endl;
            help();
            return -1;
        }
        if( inputName.empty() || (isdigit(inputName[0]) && inputName.size() == 1) )
        {
            int camera = inputName.empty() ? 0 : inputName[0] - '0';
            if(!capture.open(camera))
                cout << "Capture from camera #" <<  camera << " didn't work" << endl;
        }
        else if( inputName.size() )
        {
            image = imread( inputName, 1 );
            if( image.empty() )
            {
                if(!capture.open( inputName ))
                    cout << "Could not read " << inputName << endl;
            }
        }
        else
        {
            image = imread( "../data/lena.jpg", 1 );
            if(image.empty()) cout << "Couldn't read ../data/lena.jpg" << endl;
        }

        if( capture.isOpened() )
        {
            cout << "Video capturing has been started ..." << endl;

            for(;;)
            {
                capture >> frame;
                if( frame.empty() )
                    break;

                Mat frame1 = frame.clone();
                this->detectAndDraw( frame1, cascade, nestedCascade, scale, tryflip );

                char c = (char)waitKey(10);
                if( c == 27 || c == 'q' || c == 'Q' )
                    break;
            }
        }
        else
        {
            //cout << "Detecting face(s) in " << inputName << endl;
            if( !image.empty() )
            {
                detectAndDraw( image, cascade, nestedCascade, scale, tryflip );
                waitKey(0);
            }
            else if( !inputName.empty() )
            {
                /* assume it is a text file containing the
                list of the image filenames to be processed - one per line */
                FILE* f = fopen( inputName.c_str(), "rt" );
                if( f )
                {
                    char buf[1000+1];
                    while( fgets( buf, 1000, f ) )
                    {
                        int len = (int)strlen(buf);
                        while( len > 0 && isspace(buf[len-1]) )
                            len--;
                        buf[len] = '\0';
                        cout << "file " << buf << endl;
                        image = imread( buf, 1 );
                        if( !image.empty() )
                        {
                            this->detectAndDraw( image, cascade, nestedCascade, scale, tryflip );
                            char c = (char)waitKey(0);
                            if( c == 27 || c == 'q' || c == 'Q' )
                                break;
                        }
                        else
                        {
                            cerr << "Aw snap, couldn't read image " << buf << endl;
                        }
                    }
                    fclose(f);
                }
            }
        }

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
void OpenCVTest::help(){

        cout << "\nThis program demonstrates the cascade recognizer. Now you can use Haar or LBP features.\n"
                "This classifier can recognize many kinds of rigid objects, once the appropriate classifier is trained.\n"
                "It's most known use is for faces.\n"
                "Usage:\n"
                "./facedetect [--cascade=<cascade_path> this is the primary trained classifier such as frontal face]\n"
                   "   [--nested-cascade[=nested_cascade_path this an optional secondary classifier such as eyes]]\n"
                   "   [--scale=<image scale greater or equal to 1, try 1.3 for example>]\n"
                   "   [--try-flip]\n"
                   "   [filename|camera_index]\n\n"
                "see facedetect.cmd for one call:\n"
                "./facedetect --cascade=\"../../data/haarcascades/haarcascade_frontalface_alt.xml\" --nested-cascade=\"../../data/haarcascades/haarcascade_eye_tree_eyeglasses.xml\" --scale=1.3\n\n"
                "During execution:\n\tHit any key to quit.\n"
                "\tUsing OpenCV version " << CV_VERSION << "\n" << endl;

}
