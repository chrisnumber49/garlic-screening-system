#include "opencv2/highgui/highgui.hpp"
#include <opencv2/opencv.hpp>
#include <bcm2835.h>
#include <stdio.h>

using namespace cv;
using namespace std;

int main()
{
    int block_size = 1000;
    CvCapture *capture;
    IplImage *frame; 
    IplImage *gray;  
    IplImage *dst1;  
    IplImage *dst11;
    IplImage *dst2;
    IplImage *dst3;
    CvSeq *g_pcvSeq = NULL;	
    CvMemStorage *storage;
    
    
    capture = cvCaptureFromCAM(0) ;
    cvNamedWindow("Webcam",0);  //0 = size is changable
    cvNamedWindow( "canny", 1 );
    cvNamedWindow( "threshold", 1 );
    cvNamedWindow( "contours", 1 );
    
    while(true)
    {
        frame = cvQueryFrame(capture);	
        storage = cvCreateMemStorage(block_size);
	gray = cvCreateImage( cvGetSize(frame), IPL_DEPTH_8U, 1 );
        dst1 = cvCreateImage( cvGetSize(frame), IPL_DEPTH_8U, 1 );
	dst11 = cvCreateImage( cvGetSize(frame), IPL_DEPTH_8U, 1 );
	dst2 = cvCreateImage( cvGetSize(frame), IPL_DEPTH_8U, 1 );
	dst3 = cvCloneImage(frame);
        
	cvCvtColor( frame, gray, CV_BGR2GRAY );
	cvCanny(gray, dst1, 50, 150, 3);
	cvThreshold(gray, dst11, 65, 255, CV_THRESH_BINARY);
	cvThreshold(gray, dst2, 65, 255, CV_THRESH_BINARY);
	cvFindContours(dst2, storage, &g_pcvSeq, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));
        cvDrawContours(dst3, g_pcvSeq, cvScalar(255,255,0), cvScalar(0,0,0), 1, 1, 4, cvPoint(0, 0));
        
        cvShowImage("Webcam",frame);	
	cvShowImage( "canny", dst1 );
	cvShowImage( "threshold", dst11 );
	cvShowImage( "contours", dst3 );	
	
        if(cvWaitKey(20)>=0)
        {
		break;
        }
	cvReleaseImage(&gray);
	cvReleaseImage(&dst1);
	cvReleaseImage(&dst11);
	cvReleaseImage(&dst2);	
	cvReleaseImage(&dst3);	
	cvReleaseMemStorage(&storage);
	
    }
    cvReleaseCapture(&capture);
    cvDestroyWindow("Webcam");
    cvDestroyWindow("canny");
    cvDestroyWindow("threshold");
    cvDestroyWindow("contours");   
}