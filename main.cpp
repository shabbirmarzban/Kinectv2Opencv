#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <fstream>
#include <math.h>
#include "K4Wv2OpenCVModule.h"

using namespace cv;
using namespace std;

int main( int argc, char* argv[] )
{
	CK4Wv2OpenCVModule myKinect;
	myKinect.InitializeKinectDevice();
    for(;;){
		myKinect.UpdateData();
		Mat colorFrame = myKinect.colorRAWFrameMat;
		imshow("DepthImage",colorFrame);

		if( waitKey( 30 ) >= 0 )
            break;
	}
	
    return 0;
}

