
#include "stdafx.h"
#include "BoardCalibration.h"
#include <windows.h>

using namespace std; 

namespace BoardCalibration
{
	/* vector with bytes of frame ( Mat ) ,cause we must sometimes flush RAM */
	vector<shared_ptr<BYTE[]>>shared_of_bytes; 
	
		int count = 0;
		Mat cameraMatrix;
		Mat distanceCoefficients;
		
		 string pathName;

		 const float claibrationSquareDimension = 0.01905f; // meters
		 const float arucoSquareDimension = 0.1016f;//metrs
		 const Size chessboardDimensions = Size(6, 9); //board size

		 /*

		 1) frame - we use in order to analyze this aruco marker or not
		 2) we need to know the ID`s of our marker, we need a vector of these objects
		 3) vector of vectors of Points  
		 4) rejectedCandidates - rejected pictures :)

		---------ARUCO PART-----------
		 5) parameters - these are the parameters that we will use to determine
		 6) markers library

		 */

		 VideoCapture  *vid;

		 //2
		 vector<int> markerIds;

		 //3
		 vector<vector<Point2f>> markerConers;

		 //4
		 vector<vector<Point2f>> rejectedCandidates;

		 //5
		 DetectorParameters parameters;

		 //6
		 Ptr<aruco::Dictionary> markerDictionary;// = aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME::DICT_4X4_50);

		 vector<Vec3d> translationVectors;

	// constructor
	void ArucoBoard::Initializer()
	{
		//CameraCalibrationFile - CameraCalibrationFile of your camera ( you must do it for your camera )
		cameraMatrix = Mat::eye(3, 3, CV_64F);
		markerDictionary = aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME::DICT_4X4_50);
		pathName = "C:/Users/KAKTUS/Desktop/CameraCalibrationFile";
		vid = new VideoCapture(0);
	}




	bool ArucoBoard::loadCameraCalibration()
	{
		/*
			first we’ll create , load camera calibration method , which will load for us , our Calibration File
			by name - CameraCalibrationFile
			cameraMatrix - we will fill with information
		*/

		ifstream inStream(pathName);
		if (inStream)
		{
			uint16_t rows;
			uint16_t columns;

			inStream >> rows;
			inStream >> columns;

			cameraMatrix = Mat(Size(columns, rows), CV_64F);

			for (int r = 0; r < rows; r++)
			{
				for (int c = 0; c < columns; c++)
				{
					double read = 0.0f;
					inStream >> read;

					cameraMatrix.at<double>(r, c) = read;

				}
			}


			//Distance Coefficients
			inStream >> rows;
			inStream >> columns;

			distanceCoefficients = Mat::zeros(rows, columns, CV_64F);

			for (int r = 0; r < rows; r++)
			{
				for (int c = 0; c < columns; c++)
				{
					double read = 0.0f;
					inStream >> read;

					distanceCoefficients.at<double>(r, c) = read;
					//cout << distanceCoefficients.at<double>(r, c) << "\n";
				}
			}
			inStream.close();
			return true;

		}
		return false;

	}



	//open camera for calibration
	bool ArucoBoard::openCamera() 
	{
		if (!(*vid).isOpened())
		{
			//cout << "ERR  Camera not detected" << endl;
			return false;
		}
		return true;
	}


	//motitoring of camera
	BYTE* ArucoBoard::startWebcamMonitoring()
	{
		#pragma region MyComents

		/*
			CPP ONLY
			cameraMatrix --> calibrateCamera -->this is what we calibrated


		 1) frame - use in order to analyze this aruco marker or not
		 2) we need to know the ID`s of our marker, we need a vector of these objects
		 3) vector of vectors of Points
		 4) rejectedCandidates - rejected pictures

				ARUCO PART
		 5) parameters - these are the parameters that we will use to determine
		 6) markers library
		 7) Vec3d - because we started using the three-dimensional world 

		  LOOP -- CPP ONLY!  without C# 
		 8) loop -  will try to find aruco markers
		 9) if I can’t read the frame, I will exit from loop
		 10) detectMarkers -->  will recognize markers :)
				1 - we pass the frame since we have NO pictures with aruco marker
				2 - marker dictimory - these are the markers we are trying to find
				3 - markerConers - these are the “corners” that I want to detect
				4 - marker ids - these are numbers in the name of markers in the library from 0 to 49 4x4Marker_0.jpg

		11) we need to evaluate the camera position
			calibrateCamera -  we need more than 20 pictures taken by the camera, but better than 50

		12) draw the aruco marker axis that we found in the upper part of the image frame that we have
			a)if we want to draw something we need frame
			b) cameraMatrix - on which we will work
			c, d
		13)we need "for loop"  , since we have many different markers that can be a picture, therefore, we need to make 
			sure that we have bypassed all potential pictures and see that nothing was found on them.

		*/

		

		/*
		//1
		Mat frame;
		//2
		vector<int> markerIds;

		//3
		vector<vector<Point2f>> markerConers;

		//4
		vector<vector<Point2f>> rejectedCandidates;

		//5
		aruco::DetectorParameters parameters;

		//6
		Ptr<aruco::Dictionary> markerDictionary = aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME::DICT_4X4_50);


		VideoCapture  vid(0);
		if (!vid.isOpened()) 
		{
			cout << "ERR 5 Camera not detected" << endl;
			return -1;
		}

		namedWindow("Webcam", WINDOW_AUTOSIZE);

		//7
		vector<Vec3d> rotationVectors;
		vector<Vec3d> translationVectors;
		
			//8
			while (true)
			{
				//9
				if (!vid.read(frame))
				{
					break;
				}

				//10
				aruco::detectMarkers(frame, markerDictionary, markerConers, markerIds);

				//11
				aruco::estimatePoseSingleMarkers(markerConers, arucoSquareDimension, cameraMatrix, distanceCoefficients, rotationVectors, translationVectors);

				//13

				for (int i = 0; i < markerIds.size(); i++)
				{
					//12
					//DRAW LINES FUNCTION
					//0.1f - length of the axis
					aruco::drawAxis(frame, cameraMatrix, distanceCoefficients, rotationVectors[i], translationVectors[i], 0.1f);
			}

			imshow("Webcam", frame);
			if (waitKey(30) >= 0)
			{
				break;
			}

		*/
			#pragma endregion


	if (count > 500 )
	{
			
		rotationVectors.clear();
		rotationVectors.shrink_to_fit();
		translationVectors.clear();
		translationVectors.shrink_to_fit();
		shared_of_bytes.clear();
		count = 0;

	}
		count++;

		Mat betterImage;
		Mat frame;
		//read frame
		(*vid) >> frame;

		aruco::detectMarkers(frame, markerDictionary, markerConers, markerIds);
		aruco::estimatePoseSingleMarkers(markerConers, arucoSquareDimension, cameraMatrix, distanceCoefficients, rotationVectors, translationVectors);
				
		shared_ptr<BYTE[]> result(new BYTE[frame.cols*frame.rows * 4]);
			
		cv::flip(frame, frame, 0);

		cvtColor(frame, betterImage, COLOR_BGR2BGRA, 4);
		memcpy(result.get(), betterImage.data, betterImage.cols*frame.rows * 4);

		// we must release ALL
		betterImage.release();
		frame.release();
		shared_of_bytes.push_back(result);

		return result.get();
	}

	

	//motitoring of camera without video FOR TESTS
	void ArucoBoard::startWebcamMonitoringWithoutVideoStream()
	{
		if (count > 500)
		{
			rotationVectors.clear();
			rotationVectors.shrink_to_fit();
			translationVectors.clear();
			translationVectors.shrink_to_fit();
			shared_of_bytes.clear();
			count = 0;
		}
		count++;

		Mat frame;
		//read frame
		(*vid) >> frame;

		aruco::detectMarkers(frame, markerDictionary, markerConers, markerIds);
		aruco::estimatePoseSingleMarkers(markerConers, arucoSquareDimension, cameraMatrix, distanceCoefficients, rotationVectors, translationVectors);

	}

	

	/* ---option ONE to send data from c++ to c#----- slow option--------------*/
	void ArucoBoard::saveData()
	{
		/*

		string rotVectorName = "rotationVectorData.dat";
		string transVectorName = "transVectorData.dat";
		ofstream rotationStream(rotVectorName);

		if (rotationStream)
		{
			for (Vec3d vec : rotationVectors)
			{
				rotationStream << vec << endl;
			}

			uint16_t rows = cameraMatrix.rows;
			uint16_t columns = cameraMatrix.cols;

			rotationStream << rows << endl;
			rotationStream << columns << endl;

			for (int r = 0; r < rows; r++)
			{
				for (int c = 0; c < columns; c++)
				{
					double value = cameraMatrix.at< double>(r, c);
					rotationStream << value << endl;
				}
			}

			rows = distanceCoefficients.rows;
			columns = distanceCoefficients.cols;

			rotationStream << rows << endl;
			rotationStream << columns << endl;

			for (int r = 0; r < rows; r++)
			{
				for (int c = 0; c < columns; c++)
				{
					double value = distanceCoefficients.at< double>(r, c);
					rotationStream << value << endl;
				}
			}

				rotationStream.close();
			}
			ofstream transStream(transVectorName);

			if (transStream)
			{
				for (Vec3d vec : translationVectors)
				{
					transStream << vec << endl;
				}
				transStream.close();
			}

			*/

	}


}


