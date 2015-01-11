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

	//load ticker file
	vector<bool> timeticker;
	std::ifstream myticker("ticker.txt");
	if (!myticker.fail())
	{
		while (!myticker.eof())
		{
			bool temp;
			myticker >> temp;
			timeticker.push_back(temp);
		}

	}


	//main loop
    for(;;){
		myKinect.UpdateData();
		Mat colorFrame = myKinect.colorRAWFrameMat;
		resize(colorFrame, colorFrame, cv::Size(0, 0), 0.25, 0.25);
		INT64 timeStamp = myKinect.GetTimeStamp();
		
		int tickerLength = (int)timeticker.size() - 1;
		int currentFrame = round(DOUBLE(timeStamp) / 10000 / 1000 * 30);

		bool saveFlag = TRUE;

		if (currentFrame > tickerLength)
		{
			for (int i = 0; i < currentFrame - tickerLength-1; i++)
				timeticker.push_back(FALSE);
			timeticker.push_back(TRUE);
		
		}
		else
		{
			if (timeticker[(int)currentFrame] == 1)
				saveFlag = FALSE;
		}

		if (saveFlag){
			timeticker[(int)currentFrame] = TRUE;
			std::string fileName = "";
			fileName += std::to_string(timeStamp);
			fileName += ".jpg";
			imwrite(fileName, colorFrame);

		}


		imshow("DepthImage",colorFrame);

		if (waitKey(30) >= 0){
			//save ticker file
			bool success = FALSE;

			while (!success){

				std::ofstream myticker("ticker.txt");
				if (!myticker.fail())
				{
					for (int i = 0; i < timeticker.size(); i++)
						myticker << timeticker[i] << std::endl;
					success = TRUE;
				}
				else
				{
					remove("ticker.txt");
				}
			}
			break;
		}
	}
	




    return 0;
}

