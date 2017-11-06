#include "opencv2/highgui/highgui.hpp"
#include <opencv2/opencv.hpp>
#include <bcm2835.h>
#include <stdio.h>

using namespace cv;
using namespace std;

int main()
{
    double perimeter[12];
    int block_size = 1000;
    int i,round;
    CvCapture *capture;
    IplImage *frame;
    IplImage *img,*gray,*thre,*thre2,*dst1;
    CvSeq *g_pcvSeq, *c = NULL;	
    CvMemStorage *storage;
    CvRect rec1;
    IplROI ROI1;
    
    capture = cvCaptureFromCAM(0) ;
    cvNamedWindow("img",0);  //0 = size is changable
    cvNamedWindow( "threshold", 0 );
    cvNamedWindow( "contours", 0 );
    cvNamedWindow( "contours2", 0 );
    
    while(true)
    {
    	frame = cvQueryFrame(capture);	
		gray = cvCreateImage( cvGetSize(frame), IPL_DEPTH_8U, 1 );
    	thre = cvCreateImage( cvGetSize(frame), IPL_DEPTH_8U, 1 );
    	thre2 = cvCreateImage( cvGetSize(frame), IPL_DEPTH_8U, 1 );
    	dst1 = cvCloneImage(frame);
    	storage = cvCreateMemStorage(block_size);
	for(i=0;i<12;i++){
		perimeter[i]=1;
	}

    	cvCvtColor( frame, gray, CV_BGR2GRAY );
    	cvThreshold(gray, thre, 165, 255, CV_THRESH_BINARY);	
    	cvCopy(thre, thre2);
    
    	for(i=0;i<6;i++){
    		rec1 = cvRect(105*i,200,105,80);
    		ROI1 = cvRectToROI(rec1,1);
    		thre2->roi = &ROI1;
    		round=1;
    		cvFindContours(thre2, storage, &g_pcvSeq, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));   
    		CvContourScanner scanner = cvStartFindContours(thre2, storage, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));
    		while((c = cvFindNextContour(scanner)) != NULL)
    			{
    				double len = cvContourPerimeter(c);
				if(len > 200){
					switch(round){	
						case 1:
							perimeter[i] = len;
							break;
						case 2: 
							perimeter[i+6] = len;
							break;
						default:
							printf("you got more than 2 contours\n");
    					}
    					round++;
				}
   			 }
		if((perimeter[i]/perimeter[i+6])<6){
			printf("number %d has broken \n",i+1);
		}
		else{
			printf("number %d pass \n",i+1);
		}
   		cvDrawContours(dst1, g_pcvSeq, cvScalar(0,255,0), cvScalar(0,0,255), 1, 2, 4, cvPoint(105*i,200));
   	 }
    	thre2->roi = NULL;
    	cvShowImage("img",frame);	
    	cvShowImage( "threshold", thre );
    	cvShowImage( "contours", thre2 );
    	cvShowImage( "contours2", dst1 );
    	if(cvWaitKey(33)>=0)
        {
		break;
        }
   
    	cvReleaseImage(&gray);
    	cvReleaseImage(&thre);
    	cvReleaseImage(&thre2);
    	cvReleaseImage(&dst1);		
    	cvReleaseMemStorage(&storage);
    	
    }
    cvReleaseCapture(&capture);
    cvDestroyWindow("img");
    cvDestroyWindow("threshold");
    cvDestroyWindow("contours");
    cvDestroyWindow("contours2");
    return 0;
}
