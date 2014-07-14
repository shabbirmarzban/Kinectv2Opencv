#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <fstream>
#include <math.h>
#include "K4Wv2OpenCVModule.h"
#include <signal.h>
#define WINSIZE 300

using namespace cv;
using namespace std;
	
VideoWriter outputVideoRGB;
VideoWriter outputVideoDepth;	
ofstream myfile;


void my_handler(int s){
	printf("closing files %d\n",s);
	outputVideoRGB.release();
	outputVideoDepth.release();
	myfile.close();

    exit(0); 
}

int main( int argc, char* argv[] )
{
	signal (SIGINT,my_handler);
	CK4Wv2OpenCVModule myKinect;
	myKinect.InitializeKinectDevice();

	outputVideoRGB.open("rgbVideo.avi",-1,30,cv::Size(WINSIZE,WINSIZE));
	outputVideoDepth.open("depthVideo.avi",-1,30,cv::Size(WINSIZE,WINSIZE),false);

	myfile.open ("timeStampsForEachFrame.txt");
	
	// For measuring the timings
	int64 t2,t1,t0 = cv::getTickCount();
	double fps = 10;
	int frameProc = 0;
	double timeElapsed;
    for(;;){
		myKinect.UpdateData();
		myKinect.calculateMappedFrame();
		
		Mat colorFrame = myKinect.colorRAWFrameMat;
		
		Mat img;
		Mat dImg;
		if ((myKinect.headPointInColor.x - WINSIZE/2 >0) && (myKinect.headPointInColor.y - WINSIZE/2 >0) && (myKinect.headPointInColor.x + WINSIZE/2 < 1920 ) && (myKinect.headPointInColor.y + WINSIZE/2 < 1080)){
			Rect R = cv::Rect(myKinect.headPointInColor.x - WINSIZE/2,myKinect.headPointInColor.y - WINSIZE/2,WINSIZE,WINSIZE);
			img = myKinect.colorRAWFrameMat(R);
			dImg = myKinect.colorMappedFrameMat(R);

		}
		else{
			img = Mat::zeros( Size( WINSIZE, WINSIZE ), CV_8UC4);
			dImg = Mat::zeros( Size( WINSIZE, WINSIZE ), CV_16UC1);
		}
		cvtColor(img, img, CV_RGBA2RGB);
		outputVideoRGB << img;
		outputVideoDepth << dImg;

		//if(frameProc % 10 == 0){      
		//	t1 = cv::getTickCount();
		//	fps = 10.0 / (double(t1-t0)/cv::getTickFrequency()); 
		//	t0 = t1;
		//	cout << fps<<endl;
		//}
				
		//frameProc++;
		t1 = cv::getTickCount();
		timeElapsed =	double(t1-t0)/cv::getTickFrequency();
		myfile << timeElapsed<< endl;
		if(frameProc % 10 == 0){
			fps = 10.0 / (double(t1-t2)/cv::getTickFrequency()); 
			t2 = t1;
			cout <<fps<< endl;
		}
		frameProc++;

		//imshow("Mywindow",img);
		//cvtColor(frame, RGBA, CV_BGR2RGBA, 4);
		if( waitKey( 30 ) >= 0 ){
			outputVideoRGB.release();
			outputVideoDepth.release();
			myfile.close();
            break;
		}
	}
	
    return 0;
}

