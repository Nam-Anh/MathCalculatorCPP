#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/imgproc.hpp>
#include <vector>
#include <opencv2/core/types.hpp>
#include <opencv2/core/mat.hpp>

#include "MathDigit.h"
#include "Utility.h"

using namespace std;
using namespace cv;

class ImageProcessor {
public :
    /*--------------------------- GLOBAL VARIABLES ---------------------------*/
    int epsilon1 = 5;//For BlueStack
    int epsilon2 = 5;//For BlueStack
	/*Cropping region to crop from the sceneMat*/
	Rect croppingRegion = Rect(600, 119, 200, 50);//For BlueStack
	//Rect croppingRegion = Rect(545, 140, 250, 80);//For 5KPlayer

	/*Base height of a number 
	This variable is used to classify between numbers and signs
	Number > baseHeight*/
	int baseHeight = 12;//For BlueStack | 1st version: 16 
	//int baseHeight = 35;//For 5kPlayer

	/*This variable is used to classfify the divide sign
	If area is < 20 then this is highly the division sign*/
	int areaOfDivide = 20;//For BlueStack
	//int areaOfDivide = 30;//For 5kPlayer

	/*This variable is used to classfify the subtract sign
	If area is < 20 then this is highly the subtraction sign*/
	int areaOfSubtract = 20;//For BlueStack
	//int areaOfSubtract = 30;//For 5KPlayer
	 
    string addressTem;
    string addressSceneMat;
    string result;
    /*----------------------------- CONSTRUCTORS -----------------------------*/
    ImageProcessor();
    //Address 1 is the directory of the template folder
	//Address 2 is the directory of the sceneMat file
    ImageProcessor(string addressTem, string addSceneMat);
    /*------------------------------ PROTOTYPE -----------------------------------*/
	/*---------------------------- PRE-PROCESSING --------------------------------*/
	bool isBlack(Mat src);
    /*-------------------------  OVERALL PROCESSING  -----------------------------*/
    /*Convert from bgr image to binary image*/
    Mat Threshold(Mat img);
    vector<Mat> ThreshForObject(vector<Mat> ori_image_object);
    /*Overall processing for Object*/
    void processObject(vector<Mat> vec_img_object, vector<Mat>& numberObjects, vector<Mat>& signObjects);
    /*Overall processing for Scene*/
    void processScene(Mat img_scene, vector<Mat>& numbers, vector<Mat>& mathSigns, vector<MathDigit>& mathDigits);
    /*-------------------------- COMPARE IMAGES ----------------------------------*/
    /*Subtract each items from objNum with Mat sceneMat to find most matching pair*/
    int subtractForMinNum(vector<Mat> objMat, Mat sceneMat);

    /*Subtract each items from objSign with Mat sceneMat to find most matching pair*/
    int subtractForMinSign(vector<Mat> objMat, Mat sceneMat);

    /*Subtract two vectors to find pairs of most matching Mat
     * 1 from vector<Mat> objMat, 1 from vector<Mat> sceneMat*/
    void subtractForMinVector(vector<Mat> objMat, vector<Mat> sceneMat);

    /*Classify math signs*/
    void classifySign(Mat sceneMat);
    /*Classify math numbers*/
    void classifyNum(vector<Mat> imgObjects, Mat sceneMat);
    /*---------------------------- CONTOURS --------------------------------------*/
    /*Find Contours of image*/
    vector<vector<Point> > findC(Mat src);
    /*Find Single Moments*/
    Moments findSingleMo(vector<Point> cont);
    /*Find Moments*/
    vector<Moments> findMo(vector<vector<Point> > contours);
    /*Find Single Centers*/
    Point findSingleCen(Moments mu);
    /*Find Centers*/
    vector<Point> findCen(int contSize, vector<Moments> mu);
    /*Get center of one contour*/
    Point getCenterOfContour(vector<Point> cont);
    /*Combine co-incident contours*/
    vector<vector<Point> > combineCon(vector<vector<Point> > contours);
    /*--------------------------  EXPAND WHITE  ----------------------------------*/
    /*Using dialte function to expand white parts*/
    Mat expandForSign(Mat src);
    Mat expandForNum(Mat src);
	/*--------------------------  NARROW WHITE  ----------------------------------*/
	/*Using ERODE then DILATE function with small kernel to narrow white parts*/
	Mat narrowForNum(Mat src);
    /*------------------------ ADDITIONAL FUNCTIONS ------------------------------*/
    void quickSort(vector<vector<Point> >& contours, int left, int right);
    /*--------------------------  I/O FUNCTIONS  ---------------------------------*/
    /*Read all images*/ 
    vector<Mat> readAllFolder(int numOfImages, string folderName);
    /*Write to file*/
    void writeToFile(string i);
    /*Write to string*/
    void writeToString(string i);
    /*----------------------  OVERALL PROCESSING IMAGE  ----------------------*/
    void takeSceneMatAndReturnString(Mat src);
};


#endif /* IMAGEPROCESSOR_H */

