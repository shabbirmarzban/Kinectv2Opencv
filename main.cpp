#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <fstream>
#include <math.h>
#include "K4Wv2OpenCVModule.h"

#define WINSIZE 120
#define WINSIZECOLOR 150

using namespace cv;
using namespace std;

int main( int argc, char* argv[] )
{
	CK4Wv2OpenCVModule myKinect;
	myKinect.InitializeKinectDevice();
    for(;;){
		myKinect.UpdateData();
		Mat irFrame = myKinect.colorRAWFrameMat;
		imshow("DepthImage",irFrame);

		if( waitKey( 30 ) >= 0 )
            break;
	}
	
    return 0;
}

