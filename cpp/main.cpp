#include <opencv2/aruco.hpp>
#include <math.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>

using namespace cv;
using namespace std;



const float claibrationSquareDimension = 0.01905f; // meters
const float arucoSquareDimension = 0.1016f;//metrs
const Size chessboardDimensions = Size(6, 9);

void cameraCalibration(vector<Mat> calibrationImages, Size boardSize, float squareEdgedLength, Mat& cameraMatrix, Mat&  distanceCoefficient);
bool saveCameraCalibration(string name, Mat cameraMatrix, Mat distanceCoefficients);
void saveData(string name, vector<Vec3d>& rotation, vector<Vec3d>& trans);

int startWebcamMonitoring(const Mat & cameraMatrix, const Mat & distanceCoefficients, float arucoSquareDimension)
{

	/*
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
	if (!vid.isOpened()) {
		std::cout << "ERR 5 Camera not detected" << endl;
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

		//saveData("test", rotationVectors, translationVectors);

		//13
		Vec3d v;
		int k = 0;
		for (auto vec : rotationVectors)
		{
			for (auto id_s : markerIds)
			{
				v = vec;
				std::cout << "rotationVectors _ 0:" << vec << "  K = " << k << " marker ids" << id_s << std::endl;
				k++;
			}
		}
	
		imshow("Webcam", frame);
		if (waitKey(30) >= 0)
		{
			break;
		}
	}

	return 1;
}


void  cameraTest()
{


	Mat frame;
	const int fps = 20;
	/*
	1 : the parameter which accepts vid (0) it could be different types of image sources
		vid (int) is the number of the web camera we want to use
		so you can get up a string with the file name on your computer
	2 : check if our camera is connected
	3 : we need a loop
		these 2 lines do the same thing - insert information from the camera into the matrix:
			1) vid.read(frame)
			2) vid >> frame;
	4 : we need to show what we read
	*/

	// 1
	VideoCapture vid(0);

	//2 
	if (!vid.isOpened())
	{
		cout << "ERRR 9 " << endl;
		return;
	}

	// 3
	while (vid.read(frame))
	{
		//4 
		imshow("Webcam", frame);

		if (waitKey(1000 / fps) >= 0)
		{
			break;
		}
	}


}

#pragma region create the known position for our chess board window_2

/*
parameters:
		1) board - board size
		2)squareEdgeLength - the length of one side of the square
		3)  vector of points that we counted 
			the points that we will calculate will be stored in Point3f (X Y Z) (but this is a flat picture; therefore, all Z will be 0)  
*/
void createKnownBoardPositions(Size boardSize , float squareEdgeLength , vector<Point3f>& corners)
{
	/*
	 1  :create the KNOWING POSITION of the square
		we will embed in our corners vector 
		(we will create this vector and pass it to the function so that it would be possible to fill it
		with the points that we calculated, the points are the PREFERRED position of the squares)
	2 :  Next step - pull out the squares that have been determined from the checkerboard picture

	*/

	//1
	for (int y = 0; y < boardSize.height; y++)
	{
		for (int x = 0; x < boardSize.width; x++)
		{
			//1
			corners.push_back(Point3f(x * squareEdgeLength, y * squareEdgeLength, 0.0f));
		}
	}

}


/* this is our function counterpart to findChessboardCorners function (build in ) */
//2 : the next step is to pull out the squares that were determined from the checkerboard picture  ---->>> 	findChessboardCorners

/* 
	params:
		1) images -  pictures vector
		2) allFoundCorners -  we need a ref to a vector of vectors from Point2f , reason: these are the angles we found in the two-dimensional image
			(in fact, these are not corners, but a re-division of lines on a chessboard)
			due to the fact that this is  referance - these will be all the “corners” that we found in this function
		3) showResults - show result or not
*/
	void getChessboardCorners(vector<Mat> images , vector<vector<Point2f>>& allFoundCorners , bool showResults = false) 
{
	/*
		corners :
		corners will be created from 2 different objects. To pull out the corners of a chessboard we need all the different pictures we work with.

		What does this function do?
		she finds the potentially visible corners of a checkerboard
	*/


	/*
	1 : let's go through all the pictures
	2 : we need a buffer in order to keep all the points that we found in the picture (if we recognized them)
		pointBuf - these will be the objects that we are looking for here
	3 : we will create a function that will do all the work for as, the function will return true / false if it finds an intersection on the board 

		params:
			1) picture
			2) board size
			3) our buffer
			4) flag we are looking for either this or that
	4 : now we know whether we have found (or not) (corners) and therefore we must process this information
		if we find the "corners", we inserted this information into our "found corners" that we transmitted  --> allFoundCorners

	5 : we have one more function that makes our life easier  --> drawChessboardCorners
		she takes the corners and writes them to the picture for us parameters:
		1) iter - picture
		2) Size
		3) pointBuf  -  all the two-dimensional points that we found
		4) the pattern we found (iterator contains it)

	*/
	

	//1
	for (vector<Mat>::iterator iter = images.begin(); iter != images.end(); iter++)
	{
		//2
		vector< Point2f > pointBuf;

		//3
		bool found = findChessboardCorners(*iter, Size(9, 6), pointBuf, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE);

		//4 
		if (found)
		{
			allFoundCorners.push_back(pointBuf);
		}

		//test
		if (showResults)
		{
			//5
			drawChessboardCorners(*iter, Size(9, 6), pointBuf, found);

			imshow("Looking for Corners", *iter);
			waitKey(0);
		}
	}

}


#pragma endregion




#pragma region Aruco Markers window_1

/*
creation of 50 pictures with markers
for determining the position in space using a camera
*/
void createArucoMarkers()
{
	/*
	1 : you need to create a matrix in which we put ArUco Marker (picture with a marker)
	2 : need to create a pointer to --> dictonerrry object
		in this dictionary there are different markers, the Pointer will wander 
		through them and write or read pictures from the folder DICT_4X4_50 is a set
		of markers 4x4 in size (there are 50 in total)
	3 : create FOR  so that you can go through all the markers and rewrite them in the project folder
	4 : you need to go through and determine which marker we want to write to the matrix
		drawMarker parametrs:
			1) markerDictionary - the dictionary we created
			2) i - what we want to request (from 0 to 49)
			3)500 - the number of pixels we want
			4) ouputMarker - where do we want to write this marker
			5) where should be --> border


	5 : take our streang name and add (append) to the eye value and then use it as
		a file name to bring these strings to our folder
	*/

	//1
	Mat outputMarker;

	//2
	Ptr<aruco::Dictionary> markerDictionary = aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME::DICT_4X4_50);

	//3
	for (int i = 0; i < 50; i++)
	{
		//4
		aruco::drawMarker(markerDictionary, i, 500, outputMarker, 1);
		ostringstream convert;

		//5
		string imageName = "4x4Marker_";

		convert << imageName << i << ".jpg";
		imwrite(convert.str(), outputMarker);

	}
}


void window_1()
{
	createArucoMarkers();
}

#pragma endregion




#pragma region call from main to detect images window_3


void cameraCalibrationProcess() 
{

	/*
	a : frame - will contain our video frame information (without changes)
	b : drawToFrame- its a copy of our frame on which we will draw
	eye( x , y , type)

	distanceCoefficient - we need a matrix to preserve distances
	vector<Mat>  -- the reason why we have a vector of matrix -->  is because we defined it :)
	markerCorners - these are the points we found - “corners” (intersection of lines)
	rejectedCandidates - these are points that we did not save


	1 : vid(0 ) --> 0 is the first sourse (camera)
	2 : check if they could recognize the camera
	3 : if we cannot read frame -->  break
	4 : we need a vector that will save all the points that we found
	5 : findChessboardCorners - this is a built-in function, but we have its implementatio --> getChessboardCorners
	6 : we copy the frame data because we will draw on the frame
	7 : write down the "corners" if we found them
	8 : if we find a square - show it, if not then show the original frame
	*/

	//a
	Mat frame;
	//b
	Mat drawToFrame;

	Mat cameraMatrix = Mat::eye(3, 3, CV_64F);

	Mat distanceCoefficients;

	vector<Mat> savedImages;

	vector<vector <Point2f> > markerCorners;

	vector<vector <Point2f> > rejectedCandidates;

	int framesPerSecond = 20;
	// 1
	VideoCapture  vid(0);
	//2
	if (!vid.isOpened())
	{
		cout << "ERRR" << endl;
		return ;
	}


	namedWindow("Webcam", WINDOW_AUTOSIZE);

	while (true)
	{
		//3
		if (!vid.read(frame))
		{
			break;
		}

		//4
		vector<Vec2f> foundPoints;

		bool found = false;

		//5
		/*
			findChessboardCorners  --> this is an analogue of our function --> getChessboardCorners
		*/
		found = findChessboardCorners(frame, chessboardDimensions, foundPoints, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE);

		//6
		frame.copyTo(drawToFrame);

		//7
		drawChessboardCorners(drawToFrame, chessboardDimensions, foundPoints, found);
		//drawToFrame -- to write to this frame

		//8
		if (found)
		{
			imshow("Webcam", drawToFrame);
		}
		else
		{
			imshow("Webcam", frame);
		}
	
		char character = waitKey(1000 / framesPerSecond);

		switch (character)
		{
			case ' ':
				//save image
				if (found)
				{
					Mat temp;
					frame.copyTo(temp);
					savedImages.push_back(temp);
					cout << "save image" << endl;
				}
				break;
			case 13://enter key
				//start calibration 
				cout << "enter key" << endl;
				if(savedImages.size() > 15)
				{ 
					cout << "start calibration " << endl;
					cameraCalibration(savedImages, chessboardDimensions, claibrationSquareDimension, cameraMatrix, distanceCoefficients);
					saveCameraCalibration("CameraCalibrationFile", cameraMatrix, distanceCoefficients);
					cout << "end calibration " << endl;

				}
				break;
			case 27://esc key
				//exit
				return ;
			break;

		}

	}

}

void cameraCalibrationProcess( Mat& cameraMatrix , Mat& distanceCoefficients)
{
	/*
		a : frame - will contain our video frame information (without changes)
		b : drawToFrame- its a copy of our frame on which we will draw
		eye( x , y , type)

		distanceCoefficient - we need a matrix to preserve distances
		vector<Mat>  -- the reason why we have a vector of matrix -->  is because we defined it :)
		markerCorners - these are the points we found - “corners” (intersection of lines)
		rejectedCandidates - these are points that we did not save


		1 : vid(0 ) --> 0 is the first sourse (camera)
		2 : check if they could recognize the camera
		3 : if we cannot read frame -->  break
		4 : we need a vector that will save all the points that we found
		5 : findChessboardCorners - this is a built-in function, but we have its implementatio --> getChessboardCorners
		6 : we copy the frame data because we will draw on the frame
		7 : write down the "corners" if we found them
		8 : if we find a square - show it, if not then show the original frame
	*/

	//a
	Mat frame;
	//b
	Mat drawToFrame;


	vector<Mat> savedImages;

	vector<vector <Point2f> > markerCorners;

	vector<vector <Point2f> > rejectedCandidates;

	int framesPerSecond = 20;
	// 1
	VideoCapture  vid(0);
	//2
	if (!vid.isOpened())
	{
		cout << "ERRR" << endl;
		return;
	}


	namedWindow("Webcam", WINDOW_AUTOSIZE);
	int counter = 0;
	while (true)
	{
		//3
		if (!vid.read(frame))
		{
			cout << "ERR cameraCalibrationProcess" << endl;
			break;
		}

		//4
		vector<Vec2f> foundPoints;

		bool found = false;

		//5
		/*
			findChessboardCorners  --> this is an analogue of our function --> getChessboardCorners
		*/
		found = findChessboardCorners(frame, chessboardDimensions, foundPoints, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE);

		//6
		frame.copyTo(drawToFrame);

		//7
		drawChessboardCorners(drawToFrame, chessboardDimensions, foundPoints, found);
		
		//8
		if (found)
		{
			imshow("Webcam", drawToFrame);
		}
		else
		{
			imshow("Webcam", frame);
		}

		char character = waitKey(1000 / framesPerSecond);

		switch (character)
		{
		case ' ':
			//save image
			if (found)
			{
				Mat temp;
				frame.copyTo(temp);
				savedImages.push_back(temp);
				counter++;
				cout << "save image : " <<counter << endl;
				
			}
			break;
		case 13://enter key
			//start calibration 
			cout << "enter key" << endl;
			if (savedImages.size() > 30)
			{
				cout << "start calibration " << endl;
				cameraCalibration(savedImages, chessboardDimensions, claibrationSquareDimension, cameraMatrix, distanceCoefficients);
				saveCameraCalibration("CameraCalibrationFile", cameraMatrix, distanceCoefficients);
				cout << "end calibration " << endl;

			}
			break;
		case 27://esc key
			//exit
			return;
			break;

		}




	}




}





#pragma endregion



#pragma region window_4

/*
parameters:
	1) vector of matrixis that contains calibrated pictures that we want to use
	2) board size
	3) squareEdgedLength - one of the lengths on  "checker board "  - measure with a ruler or vernier caliper
	4) camera matrix  we need to  fill it
	5) length Coefficients
*/
	void cameraCalibration(vector<Mat> calibrationImages , Size boardSize , float squareEdgedLength ,Mat& cameraMatrix , Mat&  distanceCoefficient)
	{	
		/*
		1)  vector<vector<Point2f>> - Point2f because these are the points that we found in -->  calibtarionImade()  in two-dimensional space
		2)  we will write the function that we have already created -->  getChessboardCorners  
			 we will transfer pictures to this vector --> calibrationImages 
			 it will determine for us the difference between the points that the function found  getChessboardCorners

			 checkerBoardImageSpacePoints- we will write here
			false -  since we don’t want to see  output :)))
		3) we need create " known board positions ",  but we have to do this for every picture that we have
				therefore, create a vector of vectors POINT3F - because these are points from the real world
			 (1) - ita a size 1 , because the first time we filled it in , output create known board position function that we created earlier
		
		4) call this function
		5)  we will change the size of the matrix that we took with copy its values for each other matrix the new
			size wakes up - the number of pictures we already have 
			(or the number of two-dimensional points that we already have) and we fill it with what is on [0] place

			it wakes up again and again, and so we get a connection between the checkboard from two-dimensional space
			and three-dimensional space (what we expect)

		6) now we need to hold 2 vectors from radian and tangential vector
		7) we need a vector with distance coefficients

		*/

		//1
		vector<vector<Point2f>> checkerBoardImageSpacePoints;

		//2
		getChessboardCorners(calibrationImages, checkerBoardImageSpacePoints, false);

		//3 
		vector<vector< Point3f>> worldSpaceCornerPoints(1);

		// 4 worldSpaceCornerPoints(1) --> worldSpaceCornerPoints[1]
		createKnownBoardPositions(boardSize, squareEdgedLength, worldSpaceCornerPoints[0]);

		//5 
		worldSpaceCornerPoints.resize(checkerBoardImageSpacePoints.size(), worldSpaceCornerPoints[0]);

		//6
		vector<Mat> rVectors;
		vector<Mat> tVectros;

		//7 size --> 8,1
		distanceCoefficient = Mat::zeros(8, 1, CV_64F);

		calibrateCamera(worldSpaceCornerPoints, checkerBoardImageSpacePoints, boardSize, cameraMatrix, distanceCoefficient, rVectors, tVectros);

	}

	/*
		save from the image from the camera that have passed calibration
	*/


	//name of file
	bool saveCameraCalibration(string name, Mat cameraMatrix, Mat distanceCoefficients)
	{
		ofstream outStream(name);

		if (outStream)
		{
			uint16_t rows = cameraMatrix.rows;
			uint16_t columns = cameraMatrix.cols;

			outStream << rows << endl;
			outStream << columns << endl;

			for (int r = 0 ; r < rows ; r++)
			{
				for (int c = 0 ; c < columns ; c++)
				{
					double value = cameraMatrix.at< double>(r, c);
					outStream << value << endl;
				}
			}

			rows = distanceCoefficients.rows;
			columns = distanceCoefficients.cols;

			outStream << rows << endl;
			outStream << columns << endl;

			for (int r = 0; r < rows; r++)
			{
				for (int c = 0; c < columns; c++)
				{
					double value = distanceCoefficients.at< double>(r, c);
					outStream << value << endl;
				}
			}

			outStream.close();
			return true;
		}
		return false;
	}

#pragma endregion





#pragma region  window_5 webcam monitoring

	/*
		so we create a function that wakes up to monitor the web camera, and wakes up able to find aruco markers
		and determine the rotation, tilt and position of the object
	*/


	/*
	we are trying to make a calibration camera and load the reverse of what we calibrated and use
	this information to see aruro markers and then calculate the coordinates in 3-dimensional space (axes in 3-dimensional space)

	we have a saveCameraCalibration function that will synchronize and calibrate data from the camera, now we want to download this data in order
	to fill in aruco marker information in order to use the algorithm to find markers and move aroundaruco marker information 

	First of all, we will create a load camera calibration method that will load for us what we oscillated and saved
	
	*/
	
	bool loadCameraCalibration(string name, Mat& cameraMatrix , Mat& distanceCoefficients)
	{
		
		ifstream inStream(name);
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
				//	cout << cameraMatrix.at<double>(r, c) << "\n";

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
			//		cout << distanceCoefficients.at<double>(r, c) << "\n";
				}
			}
			inStream.close();
			return true;

		}


		return false;
	}


	



#pragma endregion



	void saveData(string name , vector<Vec3d>& rotation , vector<Vec3d>& trans)
	{


		stringstream ss;
		for (Vec3d vec : rotation)
		{
			ss << vec << endl;
		}

		String str = ss.str();
	//	cout << "stringstream  : " << str << endl;

		vector<unsigned char> c_vec;
		c_vec.assign(str.begin(), str.end());
		
		for (unsigned char u_c : c_vec)
		{
			cout << "<u_c> " << u_c << endl;

		}


		/*
				ofstream outStream(name);

					if (outStream)
					{

						for (Vec3d vec : rotation)
						{
							outStream << vec << endl;
						}
		*/

				/*
				uint16_t rows = cameraMatrix.rows;
				uint16_t columns = cameraMatrix.cols;

				outStream << rows << endl;
				outStream << columns << endl;



				for (int r = 0; r < rows; r++)
				{
					for (int c = 0; c < columns; c++)
					{
						double value = cameraMatrix.at< double>(r, c);
						outStream << value << endl;
					}
				}

				rows = distanceCoefficients.rows;
				columns = distanceCoefficients.cols;

				outStream << rows << endl;
				outStream << columns << endl;

				for (int r = 0; r < rows; r++)
				{
					for (int c = 0; c < columns; c++)
					{
						double value = distanceCoefficients.at< double>(r, c);
						outStream << value << endl;
					}
				}
				*/
/*
				outStream.close();
			}
*/
	}



int main(int argc, char** argv)
{
	cout << "---------------------------start----------------" << endl;

#pragma region MyRegion


	
	
	cout<<"---------------------------start----------------"<<endl;
	
	
	Mat cameraMatrix = Mat::eye(3, 3, CV_64F);

	Mat distanceCoefficients;

	//cameraCalibrationProcess(cameraMatrix , distanceCoefficients); // Calibration Process if you need

	loadCameraCalibration("IloveCamera", cameraMatrix, distanceCoefficients);

	//0.099 метра - lines size on screen (x ,y, z)
	startWebcamMonitoring(cameraMatrix, distanceCoefficients, 0.1f);
	
	
	

#pragma endregion

	
	//cameraTest();



	return 0;

}



