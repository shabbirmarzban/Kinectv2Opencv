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


void writeFile (CameraSpacePoint myCamPoints[100*100], int i) ;
int main( int argc, char* argv[] )
{
	CK4Wv2OpenCVModule myKinect;
	myKinect.InitializeKinectDevice();
	int i =1;
    for(;;){
		myKinect.UpdateData();
		Mat irFrame = myKinect.colorRAWFrameMat;
		imshow("DepthImage",irFrame);

		if( waitKey( 30 ) >= 0 )
            break;
	}
	
    return 0;
}

void writeFile (CameraSpacePoint myCamPoints[100*100], int i) 
{
	std::ofstream myfile;
	std::ostringstream o;
	o << "head" << i<< ".txt";
	myfile.open (o.str());
	for (int j =0 ; j < 100*100 ; j++)
		myfile << myCamPoints[j].X << " "<<myCamPoints[j].Y  << " " <<myCamPoints[j].Z << endl;
	myfile.close();

}