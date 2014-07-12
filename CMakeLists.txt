cmake_minimum_required(VERSION 2.8)
project( DisplayImage )
find_package( OpenCV REQUIRED )

set(KINECT_LIB "C:/Program Files/Microsoft SDKs/Kinect/v2.0-DevPreview1406" CACHE FILEPATH "description")
FIND_LIBRARY(Kinect20_LIBRARIES NAMES Kinect20 Kinect20.lib PATHS "${KINECT_LIB}/lib/x64" DOC "Freetype library")
FIND_PATH(Kinect20_INCLUDE_DIRS Kinect.h "${KINECT_LIB}/inc" DOC "Freetype includes")
INCLUDE_DIRECTORIES(${Kinect20_INCLUDE_DIRS})

file(GLOB K4W_OPENCV_SOURSES	
	K4Wv2OpenCVModule.*
)
add_library(KinecToOpenCV ${K4W_OPENCV_SOURSES} ${Kinect20_LIBRARIES})

add_executable( DisplayImage  main.cpp )
target_link_libraries( DisplayImage ${OpenCV_LIBS} KinecToOpenCV ${Kinect20_LIBRARIES})