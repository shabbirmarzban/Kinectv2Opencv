#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <fstream>
#include <math.h>
#include "K4Wv2OpenCVModule.h"
#define WINSIZE 300

using namespace cv;
using namespace std;

int main( int argc, char* argv[] )
{
	CK4Wv2OpenCVModule myKinect;
	myKinect.InitializeKinectDevice();
	VideoWriter outputVideoRGB;
	outputVideoRGB.open("rgbVideo.avi",-1,30,cv::Size(WINSIZE,WINSIZE));
	Mat m = Mat::zeros( Size( WINSIZE, WINSIZE ), CV_16UC1);

	// For measuring the timings
	int64 t1,t0 = cv::getTickCount();
	double fps = 10;
	int frameProc = 0;

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

		if(frameProc % 10 == 0){      
			t1 = cv::getTickCount();
			fps = 10.0 / (double(t1-t0)/cv::getTickFrequency()); 
			t0 = t1;
			cout << fps<<endl;
		}
				
		frameProc++;

		//imshow("Mywindow",m);
		//cvtColor(frame, RGBA, CV_BGR2RGBA, 4);
		if( waitKey( 30 ) >= 0 ){
			outputVideoRGB.release();
            break;
		}
	}
	
    return 0;
}

