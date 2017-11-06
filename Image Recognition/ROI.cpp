#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <bcm2835.h>
#include <stdio.h>

using namespace cv;
using namespace std;

int main()
{
    int block_size = 1000;
    int w;
    CvCapture *capture;
    IplImage *frame;   
    IplImage *dst1;     
    IplImage *dst2;
    IplImage *dst3;
    CvRect rec1,rec2,rec3;
    IplROI ROI1,ROI2,ROI3;
    
    
    capture = cvCaptureFromCAM(0) ;
    cvNamedWindow("Webcam",0);  //0 = size is changable
    cvNamedWindow( "ROI1", 1 );
    cvNamedWindow( "ROI2", 1 );
    cvNamedWindow( "ROI3", 1 );
    
    while(true)
    {
        frame = cvQueryFrame(capture);	
        dst1 = cvCloneImage(frame);
	dst2 = cvCloneImage(frame);
	dst3 = cvCloneImage(frame);
        
	rec1 = cvRect(0,0,210,480);
	ROI1 = cvRectToROI(rec1,1);
	rec2 = cvRect(210,0,210,480);
	ROI2 = cvRectToROI(rec2,1);
	rec3 = cvRect(420,0,210,480);
	ROI3 = cvRectToROI(rec3,1);

	dst1->roi = &ROI1;
	dst2->roi = &ROI2;
	dst3->roi = &ROI3;
        
        cvShowImage("Webcam",frame);	
	cvShowImage( "ROI1", dst1 );
	cvShowImage( "ROI2", dst2 );
	cvShowImage( "ROI3", dst3 );	
	
        if(cvWaitKey(20)>=0)
        {
		break;
        }
        dst1->roi = NULL;
	dst2->roi = NULL;
	dst3->roi = NULL;
	cvReleaseImage(&dst1);
	cvReleaseImage(&dst2);	
	cvReleaseImage(&dst3);	
	
    }
    cvReleaseCapture(&capture);
    cvDestroyWindow("Webcam");
    cvDestroyWindow("ROI1");
    cvDestroyWindow("ROI2");
    cvDestroyWindow("ROI3");   
}