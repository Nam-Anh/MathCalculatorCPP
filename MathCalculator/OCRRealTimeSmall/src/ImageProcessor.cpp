#include "../headers/ImageProcessor.h"

ImageProcessor::ImageProcessor() {

}

ImageProcessor::ImageProcessor(string addressTem, string addSceneMat) {
	this->addressTem = addressTem;
	this->addressSceneMat = addSceneMat;
}
/*---------------------------- PRE-PROCESSING --------------------------------*/
bool ImageProcessor::isBlack(Mat src) {
	int whiteCount = 0;
	//If input image is 3 channels -> convert to black&white image
	Mat dst;
	if (src.channels() > 1) {
		cvtColor(src, dst, CV_BGR2GRAY);
	}
	else {
		dst = src.clone();
	}
	//Find white pixel
	for (int y = 0; y < dst.rows; y++) {
		for (int x = 0; x < dst.cols; x++) {
			if (dst.at<uchar>(y, x) > 100) {
				whiteCount++;
			}
		}
	}

	return whiteCount == 0 ? true : false;
}
/*-------------------------- OVERALL PROCESSING ------------------------------*/
Mat ImageProcessor::Threshold(Mat img) {
	Mat dst;
	try {
		cvtColor(img, dst, CV_BGR2GRAY);
		threshold(dst, dst, 100, 255, THRESH_BINARY);
	}
	catch (cv::Exception & e)
	{
		cerr << e.msg << endl; // output exception message
	}
	return dst;
}

vector<Mat> ImageProcessor::ThreshForObject(vector<Mat> ori_img_object) {
	vector<Mat> res;
	try {
		/*This function take in all original image, which are still not formatted.
		After being proccessed, numbers are white and background is black*/
		for (int i = 0; i < ori_img_object.size(); i++) {
			if (ori_img_object[i].channels() > 1) {
				/*Convert from 3 channels to 1 channel*/
				cvtColor(ori_img_object[i], ori_img_object[i], CV_BGR2GRAY);
			}
			threshold(ori_img_object[i], ori_img_object[i], 100, 255, THRESH_BINARY);
			res.push_back(ori_img_object[i]);
		}
	}
	catch (cv::Exception & e)
	{
		cerr << e.msg << endl; // output exception message
	}
	
	return res;
}

/*Process input object*/
void ImageProcessor::processObject(vector<Mat> vec_img_object, vector<Mat>& numberObjects, vector<Mat>& signObjects) {
	try {
		/*---------------------------- OBJECT IMAGE ------------------------------*/
		/*Loop through all elements in the vector of object images*/
		for (int j = 0; j < vec_img_object.size(); j++) {
			Mat img_object = vec_img_object[j];
			// Approximate contours to polygons + get bounding rects
			vector<vector<Point> > contours = findC(img_object);
			vector<vector<Point> > contoursPoly(contours.size());
			vector<Rect> boundRect(contours.size());

			for (int i = 0; i < contours.size(); i++) {
				approxPolyDP(Mat(contours[i]), contoursPoly[i], epsilon1, true);
				boundRect[i] = boundingRect(Mat(contoursPoly[i]));
			}
			/*---------------- Crop image according to Bounding Rect------------------*/

			Mat temp = Mat::zeros(img_object.size(), CV_8UC3);
			//Create vector of matrix after applying bounding boxes for OBJECT
			for (int i = 0; i < boundRect.size(); i++) {
				temp = img_object(boundRect[i]);

				/*Using size of contour to distinguish different contours*/
				if (temp.rows >= 19 || temp.cols > 30) {
					numberObjects.push_back(temp);
				}
				else {
					signObjects.push_back(temp);
				}
			}
		}
	}
	catch (cv::Exception & e)
	{
		cerr << e.msg << endl; // output exception message
	}
}

/*Process input scene*/
void ImageProcessor::processScene(Mat img_scene, vector<Mat>& numbers, vector<Mat>& mathSigns, vector<MathDigit>& mathDigits) {
	try {
		/*---------------------------- SCENE IMAGE -------------------------------*/
		//Check if image is all Black
		if (isBlack(img_scene)) {
			return;
		}
		// Seperate all white parts
		img_scene = narrowForNum(img_scene);

		// Approximate contours to polygons + get bounding rects
		vector<vector<Point> > contours = findC(img_scene);
		// Arrange the whole vector of contours
		quickSort(contours, 0, contours.size() - 1);
		// Find and combine two close contours
		contours = combineCon(contours);

		// Start cropping according to the bounding Rect
		vector<vector<Point> > contours_poly(contours.size());
		vector<Rect> boundRect(contours.size());

		for (int i = 0; i < contours.size(); i++) {
			approxPolyDP(Mat(contours[i]), contours_poly[i], epsilon2, true);
			boundRect[i] = boundingRect(Mat(contours_poly[i]));
		}

		/*---------------- Crop image according to Bounding Rect------------------*/
		Mat temp = Mat::zeros(img_scene.size(), CV_8UC3);
		//Create vector of matrix after applying bounding boxes for SCENE
		for (int i = 0; i < boundRect.size(); i++) {
			/*Crop wanted part using bounding Rect*/
			temp = img_scene(boundRect[i]);
			cout << "temp: " << temp.size() << endl;
			/*Initialize new MathDigit instance*/
			MathDigit digit;
			/*Using size of contour to distinguish different contours*/
			if (temp.rows >= this->baseHeight) {
				numbers.push_back(temp);
				digit = MathDigit(i, "num", temp);
			}
			else {
				mathSigns.push_back(temp);
				digit = MathDigit(i, "sign", temp);
			}
			/*If MathDigit != null -> add them to the vector of contours*/
			if (!digit.matrix.empty()) {
				mathDigits.push_back(digit);
			}
		}
	}
	catch (cv::Exception & e)
	{
		cerr << e.msg << endl; // output exception message
	}
}
/*-------------------------- COMPARE IMAGES ----------------------------------*/

/*Subtract each items from objNum with Mat sceneMat
 *  to find most matching pair*/
int ImageProcessor::subtractForMinNum(vector<Mat> objMat, Mat sceneMat) {
	int matchMostValue, trueJ, minDist;
	/*Store index of the suitable number*/
	matchMostValue = -1;
	try {
		/*This variable to compare the difference between prototype and scene images*/
		minDist = 1000;
		for (int i = 0; i < objMat.size(); i++) {
			int currentCalc = pixNonZero(objMat[i], sceneMat);

			if (currentCalc < minDist) {
				/*Update the fitter template*/
				minDist = currentCalc;
				matchMostValue = i;
			}
			//Show the list of comparer objects
		}
	}
	catch (cv::Exception & e)
	{
		cerr << e.msg << endl; // output exception message
	}
	
	return matchMostValue;
}

/*Subtract each items from objSign with Mat sceneMat
 *  to find most matching pair*/
int ImageProcessor::subtractForMinSign(vector<Mat> objMat, Mat sceneMat) {
	int matchMostValue, trueJ, minDist;
	/*Store index of the suitable number*/
	matchMostValue = -1;

	try {
		/*Expanding Mat before processing*/
		Mat drawnMat = expandForSign(sceneMat);
		//    Mat drawnMat = sceneMat.clone();

		/*This variable to compare the difference between prototype and scene images*/
		minDist = 1000;

		/*----------------Use contour area-----------------------*/
		//Find contours
		vector<vector<Point> > contours = findC(drawnMat);
		if (contourArea(contours[0]) < this->areaOfSubtract) {
			/*Case the sign is subtract, which has minimum area and subtract's area is less than 50px*/
			matchMostValue = 1;
		}
		else {
			/*Other cases when sign is +,* */
			//Compare with plus sign
			int currentCalcPlus = pixNonZero(objMat[0], sceneMat);
			//Compare with multiple sign
			int currentCalcMul = pixNonZero(objMat[2], sceneMat);

			if (currentCalcPlus < currentCalcMul) {
				//If plus is fitter
				matchMostValue = 0;
			}
			else {
				//If multiple is fitter
				matchMostValue = 2;
			}
		}
	}
	catch (cv::Exception & e)
	{
		cerr << e.msg << endl; // output exception message
	}
	
	//cout << matchMostValue << endl;
	return matchMostValue;
}

/*Subtract two vectors to find pairs of most matching Mat
 * 1 from vector<Mat> objMat, 1 from vector<Mat> sceneMat*/
void ImageProcessor::subtractForMinVector(vector<Mat> objMat, vector<Mat> sceneMat) {
	vector<vector<int> > arr;

	cout << objMat.size() << sceneMat.size() << endl;

	for (int j = sceneMat.size() - 1; j >= 0; j--) {
		//        imshow("Osce", sceneMat[j]);
		waitKey();
	}
	/*------------------  Classify each sceneMat  ----------------------------*/
	for (int j = sceneMat.size() - 1; j >= 0; j--) {
		int trueI, trueJ, minDist;
		/*Store index of the suitable number*/
		trueI = -1;
		trueJ = -1;
		/*This variable to compare the difference between prototype and scene images*/
		minDist = 1000;
		for (int i = 0; i < objMat.size(); i++) {
			arr[i][j] = pixNonZero(objMat[i], sceneMat[j]);

			if (arr[i][j] < minDist) {
				waitKey();
				/*Update the fitter prototype*/
				minDist = arr[i][j];
				trueI = i;
				trueJ = j;
			}
		}
		//Write to file
		if (trueI != -1) {
			//cout << trueI << "\t" << minDist << endl;
			writeToFile(to_string(trueI));
			writeToString(to_string(trueI));
		}
	}
	writeToFile("\n");
}

/*Classify math signs*/
void ImageProcessor::classifySign(Mat sceneMat) {
	try {
		/*Flow of this function:
		1.Find contours
		2.Use number of contours with case of -,+,* vs :,=
		3.Use number of contour's areas with cases of :,=
		3.Use subtract-2-Mat algorithm with cases of +,*,-
		*/

		/*Expanding Mat before processing*/
		Mat drawnMat = expandForSign(sceneMat);

		/*1.Find contours*/
		vector<vector<Point> > contours;
		contours = findC(drawnMat);

		/*2.Use number of contours*/
		/*Signs have double lines is :, = */
		if (contours.size() > 1) {
			/*Combine contours*/
			contours = combineCon(contours);
			/*Using contour area to classify*/
			string res;
			if (contourArea(contours[0]) < this->areaOfDivide) {
				res = "/";
			}
			else {
				res = "=";
			}
			/*imshow("ORIginal", sceneMat);
			waitKey();*/
			writeToFile(res);
			writeToString(res);
		}
		else {
			/*Signs have single lines is -,+,* */
			/*Load template of sign :,= */
			string sPlus = this->addressTem + "10.png";
			string sSub = this->addressTem + "11.png";
			string sMul = this->addressTem + "12.png";
			Mat plus = imread(sPlus);
			Mat subtract = imread(sSub);
			Mat multiple = imread(sMul);

			/*Using subtract method to classify*/
			vector<Mat> signs;
			signs.push_back(plus);
			signs.push_back(subtract);
			signs.push_back(multiple);

			vector<Mat> numberObjects;
			vector<Mat> signObjects;
			signs = ThreshForObject(signs);
			processObject(signs, numberObjects, signObjects);

			/*Using subtractForSign function to classify signs*/
			int res = subtractForMinSign(signObjects, sceneMat);

			/*Write results to file*/
			if (res == -1) {
				/*This case means that some errors have occurred*/
				return;
			}
			else if (res == 0) {
				writeToFile("+");
				writeToString("+");
			}
			else if (res == 1) {
				writeToFile("-");
				writeToString("-");
			}
			else {
				writeToFile("*");
				writeToString("*");
			}
		}
	}
	catch (cv::Exception & e)
	{
		cerr << e.msg << endl; // output exception message
	}
}

/*Classify math numbers*/
void ImageProcessor::classifyNum(vector<Mat> imgObjects, Mat sceneMat) {
	try {
		/*Flow of this function:
		1.Process number templates
		2.Using subtractMinNum algorithm to find the most matching number
		*/

		/*---------------------------- OBJECT IMAGE ------------------------------*/

		/*1.Process number templates*/
		/*Threshold object signs*/
		imgObjects = ThreshForObject(imgObjects);

		/*Expand all objects*/
		for (int i = 0; i < imgObjects.size(); i++) {
			imgObjects[i] = expandForNum(imgObjects[i]);
		}
		/*Vectors used for processing template*/
		vector<Mat> objNumbers;
		vector<Mat> objSigns;

		processObject(imgObjects, objNumbers, objSigns);
		/*------------------------------------------------------------------------*/

		/*2.Using subtractForNum function to classify numbers*/
		int res = subtractForMinNum(objNumbers, sceneMat);

		/*Write results to file*/
		if (res == -1) {
			/*This case means that some errors have occurred*/
			return;
		}
		else {
			writeToFile(to_string(res));
			writeToString(to_string(res));
		}
	}
	catch (cv::Exception & e)
	{
		cerr << e.msg << endl; // output exception message
	}
	
}

/*---------------------------- CONTOURS --------------------------------------*/

/*Find Contours of image*/
vector<vector<Point> > ImageProcessor::findC(Mat src) {
	Mat dst = src.clone();
	//Convert image to binary image
	if (src.channels() > 1) {
		cvtColor(dst, dst, CV_BGR2GRAY);
	}
	threshold(dst, dst, 100, 255, THRESH_BINARY);

	//Start finding contours
	vector<vector<Point> > contours;
	Mat drawnMat;
	findContours(dst, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	return contours;
}

/*Find Single Moments*/
Moments ImageProcessor::findSingleMo(vector<Point> cont) {
	/// Get the moments
	Moments mu;
	mu = moments(cont, false);
	return mu;
}

/*Find Moments*/
vector<Moments> ImageProcessor::findMo(vector<vector<Point> > contours) {
	/// Get the moments
	vector<Moments> mu(contours.size());
	for (int i = 0; i < contours.size(); i++) {
		mu[i] = moments(contours[i], false);
	}
	return mu;
}

/*Find Single Centers*/
Point ImageProcessor::findSingleCen(Moments mu) {
	Point mc;
	/*Get center*/
	mc = Point(mu.m10 / mu.m00, mu.m01 / mu.m00);
	return mc;
}

/*Find Centers*/
vector<Point> ImageProcessor::findCen(int contSize, vector<Moments> mu) {
	vector<Point> mc;
	for (int i = 0; i < contSize; i++) {
		mc.push_back(Point(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00));
	}
	return mc;
}

/*Get center of one contour*/
Point ImageProcessor::getCenterOfContour(vector<Point> cont) {
	Moments mu = findSingleMo(cont);
	Point center = findSingleCen(mu);

	return center;
}

/*Combine co-incident contours*/
vector<vector<Point> > ImageProcessor::combineCon(vector<vector<Point> > contours) {
	vector<vector<Point> > res;

	try {
		//Check if the countours' size is == 2 then perform combine contours
		if (contours.size() >= 2) {
			for (int i = 0; i < contours.size() - 1; i++) {
				if (abs(getCenterOfContour(contours[i]).x - getCenterOfContour(contours[i + 1]).x) < 5) {
					//Create new contours having the same value with the current contour
					vector<Point> nCon = contours[i];
					//Add all points in the next contour to this new contour
					for (int j = 0; j < contours[i + 1].size(); j++) {
						nCon.push_back(contours[i + 1][j]);
					}
					//Add this new contour to the group of all contours
					res.push_back(nCon);
					//Increase index of the current contour + 1
					i += 1;
				}
				else {
					//This case: two continuos contours are not close to each other
					res.push_back(contours[i]);
				}
			}
			//Add the last element to the list
			res.push_back(contours[contours.size() - 1]);
		}
	}
	catch (cv::Exception & e)
	{
		cerr << e.msg << endl; // output exception message
	}

	return res;
}
/*--------------------------  EXPAND WHITE  ----------------------------------*/

/*Using dialte function with small kernel to expand white parts*/
Mat ImageProcessor::expandForSign(Mat src) {
	/*With case of subtract sign
	 Expanding Mat before processing*/
	Mat drawnMat = Mat::zeros(Size(50, 50), src.type());
	src.copyTo(drawnMat(Rect(10, 10, src.cols, src.rows)));
	/*Using dilation to expand white parts*/
	int kernel_point = 0;
	Mat element = getStructuringElement(MORPH_RECT,
		Size(kernel_point * 2 + 1, kernel_point * 2 + 1),
		Point(kernel_point, kernel_point));
	dilate(drawnMat, drawnMat, element);

	return drawnMat;
}

/*Using dialte function with small kernel to expand white parts*/
Mat ImageProcessor::expandForNum(Mat src) {
	/*With case of subtract sign
	 Expanding Mat before processing*/
	Mat drawnMat = Mat::zeros(Size(50, 50), src.type());
	src.copyTo(drawnMat(Rect(10, 10, src.cols, src.rows)));
	/*Using dilation to expand white parts*/
	int kernel_point = 0;
	Mat element = getStructuringElement(MORPH_RECT,
		Size(kernel_point * 2 + 1, kernel_point * 2 + 1),
		Point(kernel_point, kernel_point));
	dilate(drawnMat, drawnMat, element);
	//    dilate(drawnMat, drawnMat, element);

	return drawnMat;
}

/*--------------------------  NARROW WHITE  ----------------------------------*/

/*Using ERODE function with small kernel to narrow white parts*/
Mat ImageProcessor::narrowForNum(Mat src) {
	/*With case of subtract sign
	Expanding Mat before processing*/
	Mat drawnMat = src.clone();
	/*Using dilation to expand white parts*/
	int kernel_point = 1;
	Mat element = getStructuringElement(MORPH_RECT,
		Size(kernel_point * 2 + 1, kernel_point * 2 + 1),
		Point(kernel_point, kernel_point));

	erode(drawnMat, drawnMat, element);
	dilate(drawnMat, drawnMat, element);

	return drawnMat;
}
/*--------------------------- Additional functions ---------------------------*/

/*QuickSort*/
void ImageProcessor::quickSort(vector<vector<Point> >& contours, int left, int right) {
	try {
		int i = left, j = right;
		int pivot = (int)(getCenterOfContour(contours[(int)(left + right) / 2]).x);

		while (i <= j) {
			while (getCenterOfContour(contours[i]).x < pivot) {
				i++;
			}
			while (getCenterOfContour(contours[j]).x > pivot) {
				j--;
			}
			if (i <= j) {
				vector<Point> temp;
				temp = contours[i];
				contours[i] = contours[j];
				contours[j] = temp;
				i++;
				j--;
			}
		};
		if (left < j) {
			quickSort(contours, left, j);
		}
		if (i < right) {
			quickSort(contours, i, right);
		}
	}
	catch (cv::Exception & e)
	{
		cerr << e.msg << endl; // output exception message
	}


}
/*--------------------------  I/O FUNCTIONS  ---------------------------------*/

/*Read the whole folder*/
vector<Mat> ImageProcessor::readAllFolder(int numOfImages, string folderName) {
	vector<Mat> results;
	Mat currentMat;
	try {
		for (int i = 0; i < numOfImages; i++) {
			string fileName = this->addressTem + to_string(i) + ".png";
			currentMat = imread(fileName);

			/*Un-comment this line to check*/
			/*cout << fileName << "|" << currentMat.size() << endl;*/

			results.push_back(currentMat);
		}

	}
	catch (cv::Exception & e)
	{
		cerr << e.msg << endl; // output exception message
	}
	return results;
}

/*Write result to file*/
void ImageProcessor::writeToFile(string i) {
	try {
		ofstream stream;
		stream.open("../result.txt", ios_base::app);
		if (!stream.is_open()) {
			cout << "Can not find the wanted file\n";
		}
		stream << i;

		stream.close();
	}
	catch (cv::Exception & e)
	{
		cerr << e.msg << endl; // output exception message
	}
}

/*Write to string*/
void ImageProcessor::writeToString(string i) {
	try {
		this->result += i;
	}
	catch (cv::Exception & e)
	{
		cerr << e.msg << endl; // output exception message
	}

}

/*----------------------  OVERALL PROCESSING IMAGE  ----------------------*/
void ImageProcessor::takeSceneMatAndReturnString(Mat src) {
	try {
		/*Number of prototype images
		1->9 +,-,x,:, = */
		int numberOfObject = 15;
		vector<Mat> ori_img_object = readAllFolder(numberOfObject, this->addressTem);
		Mat ori_img_scene = src;

		//-- Step 0: Pre processing
		//The is the Mat that we need to process
		Mat img_scene = Threshold(ori_img_scene);
		//This is the vector containing all template
		vector<Mat> imgObjects = ThreshForObject(ori_img_object);
		/*------------------------------------------------------------------------*/
		//-- Step 1: Crop scene 
		img_scene = img_scene(this->croppingRegion);

		threshold(img_scene, img_scene, 100, 255, THRESH_BINARY);

		//-- Step 2: Process and draw output onto image
		/*Vectors used for processing scene*/
		vector<Mat> Numbers;
		vector<Mat> Signs;
		/*Vectors used for storing results after processing scene*/
		vector<MathDigit> mathDigits;
		/*---------------------------- SCENE IMAGE -------------------------------*/
		processScene(img_scene, Numbers, Signs, mathDigits);
		/*------------------------------------------------------------------------*/

		/*------------------ SUBTRACT TO RECOGNIZE OBJECTS -----------------------*/
		//imshow("image", img_scene);

		/*Loop through all MathDigit objects and find the most matching template with it*/
		for (int i = 0; i < mathDigits.size(); i++) {
			MathDigit digit = mathDigits[i];

			if (!digit.matrix.empty()) {
				/*If MathDigit is number*/
				if (digit.type.compare("num") == 0) {
					/*Compare to recognize numbers*/
					classifyNum(imgObjects, digit.matrix);
				}
				/*If MathDigit is sign*/
				if (digit.type.compare("sign") == 0) {
					/*Compare to recognize numbers*/
					classifySign(digit.matrix);
				}
			}
		}
		writeToFile("\n");
	}
	catch (cv::Exception & e)
	{
		cerr << e.msg << endl; // output exception message
	}


}