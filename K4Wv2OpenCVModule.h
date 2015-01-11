#pragma once
// Default IO
#include <iostream>
// OpenCV
#include <opencv2\opencv.hpp>
// Kinect for Windows SDK 2.0
#include <Kinect.h>
#include <Kinect.Face.h>

/*
#ifdef _DEBUG
#pragma comment( lib, "opencv_core248d.lib" )
#pragma comment( lib, "opencv_highgui248d.lib" )
#pragma comment( lib, "opencv_imgproc248d.lib" )
#else
#pragma comment( lib, "opencv_core248.lib" )
#pragma comment( lib, "opencv_highgui248.lib" )
#pragma comment( lib, "opencv_imgproc248.lib" )
#endif
*/

#pragma comment( lib, "Kinect20.lib" )
#pragma comment( lib, "Kinect20.Face.lib" )


// Namespaces
using namespace std;
using namespace cv;


class CK4Wv2OpenCVModule
{
	// Sensor frame data values
	// Color frame resolution
	static const int COLOR_FRAME_WIDTH = 1920;
	static const int COLOR_FRAME_HEIGHT = 1080;
	// Depth frame resolution
	static const int DEPTH_FRAME_WIDTH = 512;
	static const int DEPTH_FRAME_HEIGHT = 424;
	// Infrared frame resolution
	static const int INFRARED_FRAME_WIDTH = 512;
	static const int INFRARED_FRAME_HEIGHT = 424;
	
	bool provideColor;
	bool provideBody;
	bool provideFace;

	INT64                   m_nStartTime;
	INT64                   timeStamp;

public:
	CK4Wv2OpenCVModule();
	CK4Wv2OpenCVModule(bool provideColor, bool provideBody, bool provideFace);
	~CK4Wv2OpenCVModule();

	HRESULT InitializeKinectDevice();

	// Image frame Mat
	Mat colorRAWFrameMat;

	// Process frame
	void UpdateData();
	INT64 GetTimeStamp();

	// Calculate Mapped Frame

	RGBQUAD* pColorRAWBuffer;
	
	//ushort* pDepthRAWBuffer;
	//ushort* pInfraRAWBuffer;
private:
	// Device
	IKinectSensor* pSensor;
	

	// Frame reader
	IMultiSourceFrameReader* pMultiSourceFrameReader;
	//Face Readers

	// Face sources
	IFaceFrameSource*		m_pFaceFrameSources[BODY_COUNT];

	// Face readers
	IFaceFrameReader*		m_pFaceFrameReaders[BODY_COUNT];

	// HD Face Sources
	IHighDefinitionFaceFrameSource* m_pHDFaceFrameSources[BODY_COUNT];

	// HDFace readers
	IHighDefinitionFaceFrameReader*		m_pHDFaceFrameReaders[BODY_COUNT];

	// Release function
	template< class T > void SafeRelease( T** ppT );

};

