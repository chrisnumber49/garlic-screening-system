#include "opencv2/highgui/highgui.hpp"
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>
#include <wiringPi.h>
#include <pthread.h>

#define out0  0
#define out1  1
#define out2  2
#define out3  3
#define out4  4
#define out5  5

void *GPIO1(void *ptr);
void *GPIO2(void *ptr);
void *GPIO3(void *ptr);
void *GPIO4(void *ptr);
void *GPIO5(void *ptr);
void *GPIO6(void *ptr);

using namespace cv;
using namespace std;

int control1=0,control2=0,control3=0,control4=0,control5=0,control6=0;

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

    
    
    wiringPiSetup();
    pinMode(out0,OUTPUT);
    pinMode(out1,OUTPUT);
    pinMode(out2,OUTPUT);
    pinMode(out3,OUTPUT);
    pinMode(out4,OUTPUT);
    pinMode(out5,OUTPUT);

    digitalWrite(out0,HIGH);
    digitalWrite(out1,HIGH);
    digitalWrite(out2,HIGH);
    digitalWrite(out3,HIGH);
    digitalWrite(out4,HIGH);
    digitalWrite(out5,HIGH);
    
    pthread_t th1,th2,th3,th4,th5,th6;
    char *message = "lane";
    pthread_create( &th1, NULL, GPIO1, (void*)message);
    pthread_create( &th2, NULL, GPIO2, (void*)message);
    pthread_create( &th3, NULL, GPIO3, (void*)message);
    pthread_create( &th4, NULL, GPIO4, (void*)message);
    pthread_create( &th5, NULL, GPIO5, (void*)message);
    pthread_create( &th6, NULL, GPIO6, (void*)message);
    
    capture = cvCaptureFromCAM(0) ;
    //0 = size is changable
    cvNamedWindow( "threshold", 0 );
    cvNamedWindow( "contours2", 0 );
    
    while(true)
    {
	if( capture )
        {
    	frame = cvQueryFrame(capture);	
		gray = cvCreateImage( cvGetSize(frame), IPL_DEPTH_8U, 1 );
		uchar* data = (uchar *)gray->imageData;
		int step = gray->widthStep/sizeof(uchar);
    	thre = cvCreateImage( cvGetSize(frame), IPL_DEPTH_8U, 1 );
    	thre2 = cvCreateImage( cvGetSize(frame), IPL_DEPTH_8U, 1 );
    	dst1 = cvCloneImage(frame);
    	storage = cvCreateMemStorage(block_size);
	
	for(i=0;i<12;i++){
		perimeter[i]=1;
	}

    	cvCvtColor( frame, gray, CV_BGR2GRAY );
    	for(i=0; i<gray->height; i++){
		for(j=0; j<gray->width; j++){
			if(data[i*step+j]<=110)
            {
			  data[i*step+j]=0;
            }
            else
            {
			   	data[i*step+j]=255;
            }
		}
		}
    	//cvThreshold(gray, thre, 80, 255, CV_THRESH_BINARY);
		cvDilate(gray, thre, 0, 1);
		cvErode(thre, thre, 0, 1);
		
    	cvCopy(thre, thre2);
    
    	for(i=0;i<6;i++){
    		rec1 = cvRect(106*i,165,106,150);
    		ROI1 = cvRectToROI(rec1,1);
    		thre2->roi = &ROI1;
    		round=1;
    		cvFindContours(thre2, storage, &g_pcvSeq, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));   
    		CvContourScanner scanner = cvStartFindContours(thre2, storage, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));
    		while((c = cvFindNextContour(scanner)) != NULL)
    			{
    				double len = cvContourPerimeter(c);
				if(len > 20){
					switch(round){	
						case 1:
							perimeter[i] = len;
							break;
						case 2: 
							perimeter[i+6] = len;
							break;
						default:
							;
    					}
    					round++;
				}
   			 }
		if(((perimeter[i]/perimeter[i+6])<15) && ((perimeter[i]/perimeter[i+6])!=1)){
			switch(i){
				case 0:
					control1=1;
					break;
				case 1:
					control2=1;
					break;
				case 2:
					control3=1;
					break;
				case 3:
					control4=1;
					break;
				case 4:
					control5=1;
					break;
				case 5:
					control6=1;
					break;
			}
		}
		else{
			switch(i){
				case 0:
					control1=0;
					break;
				case 1:
					control2=0;
					break;
				case 2:
					control3=0;
					break;
				case 3:
					control4=0;
					break;
				case 4:
					control5=0;
					break;
				case 5:
					control6=0;
					break;
			}
		}
   		cvDrawContours(dst1, g_pcvSeq, cvScalar(0,255,0), cvScalar(0,0,255), 1, 2, 4, cvPoint(106*i,165));
   	 }
    	thre2->roi = NULL;
	cvShowImage("threshlod",thre);	
    	cvShowImage( "contours2", dst1 );
    	if(cvWaitKey(30)==27)
        {
		break;
        }
    	cvReleaseImage(&gray);
    	cvReleaseImage(&thre);
    	cvReleaseImage(&thre2);
    	cvReleaseImage(&dst1);		
    	cvReleaseMemStorage(&storage);
    	}
    	else
        {
            cvWaitKey(30);
            continue;
        }  	
    }
    pthread_join(th1,NULL);
    pthread_join(th2,NULL);
    pthread_join(th3,NULL);
    pthread_join(th4,NULL);
    pthread_join(th5,NULL);
    pthread_join(th6,NULL);    
    cvReleaseCapture(&capture);
    cvDestroyWindow("img");
    cvDestroyWindow("threshold");
    cvDestroyWindow("contours2");
    return 0;
}

void *GPIO1(void *ptr){
    char *mg1;
    mg1 = (char *) ptr;
    while(1){
    	if(control1>0){
    		delay(500);
    		digitalWrite(out0,LOW);
    		delay(200);
    		digitalWrite(out0,HIGH);
    		printf("%s 1 has done\n",mg1);
    	}
    	delay(10);
    }
}


void *GPIO2(void *ptr){
    char *mg1;
    mg1 = (char *) ptr;
    while(1){
    	if(control2>0){
    		delay(500);
    		digitalWrite(out1,LOW);
    		delay(200);
    		digitalWrite(out1,HIGH);
    		printf("%s 2 has done\n",mg1);
    	}
    	delay(10);
    }
}

void *GPIO3(void *ptr){
    char *mg1;
    mg1 = (char *) ptr;
    while(1){
    	if(control3>0){
    		delay(500);
    		digitalWrite(out2,LOW);
    		delay(200);
    		digitalWrite(out2,HIGH);
    		printf("%s 3 has done\n",mg1);
    	}
    	delay(10);
    }
}

void *GPIO4(void *ptr){
    char *mg1;
    mg1 = (char *) ptr;
    while(1){
    	if(control4>0){
    		delay(500);
    		digitalWrite(out3,LOW);
    		delay(200);
    		digitalWrite(out3,HIGH);
    		printf("%s 4 has done\n",mg1);
    	}
    	delay(10);
    }
}

void *GPIO5(void *ptr){
    char *mg1;
    mg1 = (char *) ptr;
    while(1){
    	if(control5>0){
    		delay(500);
    		digitalWrite(out4,LOW);
    		delay(200);
    		digitalWrite(out4,HIGH);
    		printf("%s 5 has done\n",mg1);
    	}
    	delay(10);
    }
}

void *GPIO6(void *ptr){
    char *mg1;
    mg1 = (char *) ptr;
    while(1){
    	if(control6>0){
    		delay(500);
    		digitalWrite(out5,LOW);
    		delay(200);
    		digitalWrite(out5,HIGH);
    		printf("%s 6 has done\n",mg1);
    	}
    	delay(10);
    }
}
