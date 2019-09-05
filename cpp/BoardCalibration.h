#pragma once

#include <opencv2/aruco.hpp>

#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>

#include <iostream>
#include <cstdlib>
#include <stdint.h>
#include <sstream>
#include <fstream>
#include "minwindef.h"




#ifdef BOARDCALIBRATION_EXPORTS  
#define BOARDCALIBRATION_EXPORTS_API __declspec(dllexport)   
#else  
#define BOARDCALIBRATION_EXPORTS_API __declspec(dllimport)   
#endif 


using namespace cv;
using namespace std;
using namespace aruco;
 

  vector<Vec3d> rotationVectors;
//vector<Vec3d> translationVectors;



//Vector Struct
struct Vector3D
{
	//constructor
	Vector3D(float x, float y, float z)
		: X(x), Y(y), Z(z) { }
	float X, Y, Z;
};

//get rotation vector size
extern "C" void __declspec(dllexport) __stdcall getRotationVectorsAndSize(Vector3D* outRotationVector , int& outRotVectorSize)
{
	int size  = rotationVectors.size();
	outRotVectorSize = size;

	int i = 0;
	for (Vec3d vec : rotationVectors)
	{
		outRotationVector[i] =  Vector3D ( (float)vec[0],(float)vec[1],(float)vec[2]);
		
		i++;
	}
}

/*if we need sent data from c++ to c# Transform Vector*/
/*
extern "C" void __declspec(dllexport) __stdcall getTranslationVectorsAndSize(Vector3D* outTranslationVector , int& outTransVectorSize)
{
	int size = translationVectors.size();
	outTransVectorSize = size;
	int i = 0;
	for (Vec3d vec : translationVectors)
	{
		outTranslationVector[i] = Vector3D((float)vec[0], (float)vec[1], (float)vec[2]);
		
		i++;
	}
}
*/

namespace BoardCalibration
{

	class ArucoBoard
	{
	public:
		
		//constructor
		static BOARDCALIBRATION_EXPORTS_API void Initializer();

		//load data of camera that we calirating
		//we need spesial file named "CameraCalibrationFile"
		static BOARDCALIBRATION_EXPORTS_API bool loadCameraCalibration();

		//open camera and init
		static BOARDCALIBRATION_EXPORTS_API bool openCamera();

		//statr monitoring of camera
		static BOARDCALIBRATION_EXPORTS_API BYTE* startWebcamMonitoring();

		//start video monitoring without vide stream
		static BOARDCALIBRATION_EXPORTS_API void startWebcamMonitoringWithoutVideoStream();

		//we dont wont to use this function 
		static BOARDCALIBRATION_EXPORTS_API void saveData();
		
	};

}