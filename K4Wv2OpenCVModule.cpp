#include "K4Wv2OpenCVModule.h"

// Log tag
static const string TAG = "[K4WOCV]";

CK4Wv2OpenCVModule::CK4Wv2OpenCVModule():
provideColor(TRUE),
provideBody(FALSE),
provideFace(FALSE),
pSensor(NULL),
pMultiSourceFrameReader(NULL),
m_nStartTime(0),
timeStamp(0)
{
	cout << TAG << "Initializing Kinect for Windows OpenCV Module." << endl;

	//pSensor = nullptr;
	//pCoordinateMapper = nullptr;
	//pMultiSourceFrameReader = nullptr;

	// Allocate buffers
	pColorRAWBuffer = new RGBQUAD[COLOR_FRAME_WIDTH * COLOR_FRAME_HEIGHT];
	//pDepthRAWBuffer = new ushort[DEPTH_FRAME_WIDTH * DEPTH_FRAME_HEIGHT];
	//pInfraRAWBuffer = new ushort[INFRARED_FRAME_WIDTH * INFRARED_FRAME_HEIGHT];


	// Set 0
	memset( pColorRAWBuffer, 0, COLOR_FRAME_WIDTH * COLOR_FRAME_HEIGHT * sizeof( RGBQUAD ) );
	//memset( pColorRAWBuffer, 0, DEPTH_FRAME_WIDTH * DEPTH_FRAME_HEIGHT * sizeof( ushort ) );
	//memset( pColorRAWBuffer, 0, INFRARED_FRAME_WIDTH * INFRARED_FRAME_HEIGHT * sizeof( ushort ) );

	// Set Mat
	colorRAWFrameMat = Mat( Size( COLOR_FRAME_WIDTH, COLOR_FRAME_HEIGHT ), CV_8UC4, (void*)pColorRAWBuffer );
	//depthRAWFrameMat = Mat( Size( DEPTH_FRAME_WIDTH, DEPTH_FRAME_HEIGHT ), CV_16UC1, (void*)pDepthRAWBuffer );
	//infraRAWFrameMat = Mat( Size( INFRARED_FRAME_WIDTH, INFRARED_FRAME_HEIGHT ), CV_16UC1, (void*)pInfraRAWBuffer );
}

CK4Wv2OpenCVModule::CK4Wv2OpenCVModule(bool provideColor, bool provideBody, bool provideFace) :
provideColor(provideColor),
provideBody(provideBody),
provideFace(provideFace),
pSensor(NULL),
pMultiSourceFrameReader(NULL)
{
	cout << TAG << "Initializing Kinect for Windows OpenCV Module." << endl;


	// Allocate buffers
	pColorRAWBuffer = new RGBQUAD[COLOR_FRAME_WIDTH * COLOR_FRAME_HEIGHT];
	//pDepthRAWBuffer = new ushort[DEPTH_FRAME_WIDTH * DEPTH_FRAME_HEIGHT];
	//pInfraRAWBuffer = new ushort[INFRARED_FRAME_WIDTH * INFRARED_FRAME_HEIGHT];

	
	// Set 0
	memset(pColorRAWBuffer, 0, COLOR_FRAME_WIDTH * COLOR_FRAME_HEIGHT * sizeof(RGBQUAD));
	//memset(pColorRAWBuffer, 0, DEPTH_FRAME_WIDTH * DEPTH_FRAME_HEIGHT * sizeof(ushort));
	//memset(pColorRAWBuffer, 0, INFRARED_FRAME_WIDTH * INFRARED_FRAME_HEIGHT * sizeof(ushort));

	// Set Mat
	colorRAWFrameMat = Mat(Size(COLOR_FRAME_WIDTH, COLOR_FRAME_HEIGHT), CV_8UC4, (void*)pColorRAWBuffer);
	//depthRAWFrameMat = Mat(Size(DEPTH_FRAME_WIDTH, DEPTH_FRAME_HEIGHT), CV_16UC1, (void*)pDepthRAWBuffer);
	//infraRAWFrameMat = Mat(Size(INFRARED_FRAME_WIDTH, INFRARED_FRAME_HEIGHT), CV_16UC1, (void*)pInfraRAWBuffer);
}

CK4Wv2OpenCVModule::~CK4Wv2OpenCVModule()
{
	cout << TAG << "Releasing Kinect for Windows OpenCV Module." << endl;
	// Release buffers
	if ( pColorRAWBuffer )
	{
		delete pColorRAWBuffer;
		pColorRAWBuffer = nullptr;
	}

	if ( pSensor )
	{
		pSensor->Close();
		SafeRelease( &pSensor );
	}
}

HRESULT CK4Wv2OpenCVModule::InitializeKinectDevice()
{
	HRESULT hr;

	// Get default sensor
	hr = GetDefaultKinectSensor( &pSensor );
	if ( FAILED( hr ) )
	{
		cerr << TAG << "Sensor initialization error at - " << __FUNCTIONW__ << endl;
		return hr;
	}

	if ( pSensor )
	{
		// Get coordinate mapper
		// Open sensor
		hr = pSensor->Open();
		if ( SUCCEEDED( hr ) )
		{
			char frameFlg = 0;

			if (provideColor)
				frameFlg |= FrameSourceTypes_Color;
			if (provideBody)
				frameFlg |= FrameSourceTypes_Body;
			//frameFlg = FrameSourceTypes_Infrared | FrameSourceTypes_Color | FrameSourceTypes_Depth | FrameSourceTypes_Body;

			hr = pSensor->OpenMultiSourceFrameReader(frameFlg, &pMultiSourceFrameReader);
		}
	}

	if ( !pSensor || FAILED( hr ) )
	{
		cerr << TAG << "No devices ready." << endl;
		return E_FAIL;
	}

	return hr;
}

void CK4Wv2OpenCVModule::UpdateData()
{
	if ( !pMultiSourceFrameReader )
		return;

	IMultiSourceFrame* pMultiSourceFrame = nullptr;
	IDepthFrame* pDepthFrame = nullptr;
	IColorFrame* pColorFrame = nullptr;
	IInfraredFrame* pInfraFrame = nullptr;
	IBodyFrame* pBodyFrame = nullptr;

	HRESULT hr = pMultiSourceFrameReader->AcquireLatestFrame( &pMultiSourceFrame );
	
	if ( SUCCEEDED( hr ) )
	{
		// Receive color
		IColorFrameReference* pColorFrameReference = nullptr;

		HRESULT hrColor = pMultiSourceFrame->get_ColorFrameReference( &pColorFrameReference );
		pColorFrameReference->get_RelativeTime(&timeStamp);
		
		if (!m_nStartTime)
		{
			m_nStartTime = timeStamp;
		}

		if ( SUCCEEDED( hrColor ) )
		{
			hrColor = pColorFrameReference->AcquireFrame( &pColorFrame );
		}
		SafeRelease( &pColorFrameReference );


		// Receive Body Frame
		IBodyFrameReference* pBodyFrameReference = nullptr;

		HRESULT hrBody = pMultiSourceFrame->get_BodyFrameReference( &pBodyFrameReference );
		if ( SUCCEEDED( hrBody ) )
		{
			hrBody  = pBodyFrameReference->AcquireFrame(&pBodyFrame);
			INT64 nTime = 0;
			IBody* ppBodies[BODY_COUNT] = {0};
			if (SUCCEEDED(hrBody)){
				hrBody = pBodyFrame->get_RelativeTime(&nTime);
			}
			if (SUCCEEDED(hrBody))
			{
				hrBody = pBodyFrame->GetAndRefreshBodyData(_countof(ppBodies), ppBodies);
			}
			if (SUCCEEDED(hrBody)){
				for (int i = 0; i < BODY_COUNT; ++i){

					IBody* pBody = ppBodies[i];
					BOOLEAN bTracked = false;
					hrBody = pBody->get_IsTracked(&bTracked);
					if (SUCCEEDED(hrBody) && bTracked){
						//Joint joints[JointType_Count]; 
						//DepthSpacePoint depthPoint = {0};
						//ColorSpacePoint colorPoint = {0};
						//pBody->GetJoints(_countof(joints), joints);
						//pCoordinateMapper->MapCameraPointToDepthSpace(joints[JointType_Head].Position, &depthPoint);
						//pCoordinateMapper->MapCameraPointToColorSpace(joints[JointType_Head].Position, &colorPoint);
						//
						////cout << depthPoint.X  << " " <<depthPoint.Y << endl;
						//headPointInDepth.x =  depthPoint.X;
						//headPointInDepth.y =  depthPoint.Y;
						//headPointInColor.x =  colorPoint.X;
						//headPointInColor.y =  colorPoint.Y;
						//i = BODY_COUNT;

					}
				}

			}

			for (int i = 0; i < _countof(ppBodies); ++i)
			{
				SafeRelease(&ppBodies[i]);
			}
		}
		SafeRelease( &pBodyFrameReference );


		// Check color & depth
		if ( SUCCEEDED( hrColor ))
		{
			unsigned int colorBufferSize = COLOR_FRAME_WIDTH * COLOR_FRAME_HEIGHT * sizeof(RGBQUAD);
			hr = pColorFrame->CopyConvertedFrameDataToArray(colorBufferSize, reinterpret_cast< BYTE* >(pColorRAWBuffer), ColorImageFormat_Bgra);

		}


		SafeRelease( &pColorFrame );
		SafeRelease( &pBodyFrame);
		SafeRelease( &pMultiSourceFrame );

	}
}


template< class T > void CK4Wv2OpenCVModule::SafeRelease( T** ppT )
{
	if ( *ppT )
	{
		( *ppT )->Release();
		*ppT = nullptr;
	}
}

INT64 CK4Wv2OpenCVModule::GetTimeStamp(){
	return timeStamp-m_nStartTime;

}