#include "opencv2/highgui/highgui.hpp"
#include <opencv2/opencv.hpp>
#include <bcm2835.h>
#include <stdio.h>

using namespace cv;
using namespace std;

int main()
{
    int block_size = 1000;
    int i,round;
    double perimeter[12] = {1};
    char fileName[] = "/home/pi/Pictures/jaja.jpg";
    IplImage *img,*gray,*thre,*thre2,*dst1;
    CvSeq *g_pcvSeq, *c = NULL;	
    CvMemStorage *storage;
    CvRect rec1,rec2,rec3;
    IplROI ROI1,ROI2,ROI3;
    
    img = cvLoadImage(fileName);
    gray = cvCreateImage( cvGetSize(img), IPL_DEPTH_8U, 1 );
    thre = cvCreateImage( cvGetSize(img), IPL_DEPTH_8U, 1 );
    thre2 = cvCreateImage( cvGetSize(img), IPL_DEPTH_8U, 1 );
    dst1 = cvCloneImage(img);
    storage = cvCreateMemStorage(block_size);

    cvNamedWindow("img",0);  //0 = size is changable
    cvNamedWindow( "threshold", 0 );
    cvNamedWindow( "contours", 0 );
    cvNamedWindow( "contours2", 0 );
    
    cvCvtColor( img, gray, CV_BGR2GRAY );
    cvThreshold(gray, thre, 165, 255, CV_THRESH_BINARY);	
    cvCopy(thre, thre2);
    
    for(i=0;i<6;i++){
    		rec1 = cvRect(100+i*300,580,300,320);
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
		if((perimeter[i]/perimeter[i+6])<5){
			printf("number %d has broken \n",i+1);
		}
		else{
			printf("number %d pass \n",i+1);
		}
   		cvDrawContours(dst1, g_pcvSeq, cvScalar(0,255,0), cvScalar(0,0,255), 1, 2, 4, cvPoint(100+i*300, 580));
    }
    thre2->roi = NULL;
    cvShowImage("img",img);	
    cvShowImage( "threshold", thre );
    cvShowImage( "contours", thre2 );
    cvShowImage( "contours2", dst1 );
    
    cvWaitKey(0);
    system("pause");
    cvReleaseImage(&img);
    cvReleaseImage(&gray);
    cvReleaseImage(&thre);
    cvReleaseImage(&thre2);
    cvReleaseImage(&dst1);
		
    cvReleaseMemStorage(&storage);
    return 0;
}