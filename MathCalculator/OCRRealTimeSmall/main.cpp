#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <windows.h>

#include "opencv2/imgcodecs.hpp"
#include <opencv2/imgproc.hpp>
#include <vector>
#include <opencv2/core/types.hpp>
#include <opencv2/core/mat.hpp>

#include "headers/ImageProcessor.h"
#include "headers/Utility.h"
#include "headers/ImageProcessor.h"
#include "headers/MathExpression.h"
#include "headers/WindowController.h"
#include "headers/AutoClick.h"
#include "headers/RandomGenerator.h"

#include <Windows.h>

using namespace std;
using namespace cv;


#define HOTKEY1 1
#define UPPERPOINTTRUE Point(-1,-1)
#define LOWERPOINTTRUE Point(-1,-1)

#define UPPERPOINTFALSE Point(-1,-1)
#define LOWERPOINTFALSE Point(-1,-1)

/*----------------------------- GUIDELINES -----------------------------------*/
/*Some initial instructions about running this program*/
void instruction();
/*----------------------------------------------------------------------------*/

int main(int argc, char** argv) {
	instruction();

	//By default, this variable is false means that game has not started
	bool startGame = false;
	int sleepingInterval = 1500;
	int currentQues = 1;
	/*Register hotKeys so there are two hotKeys */
	RegisterHotKey(NULL, HOTKEY1, MOD_ALT | MOD_NOREPEAT, 0x53); // ALT+s
																 /*Initilize one message to get message from system*/
	MSG msg = { 0 };

	/*GetMessage return > 0 if msg is not null*/
	if (GetMessage(&msg, NULL, 0, 0) != 0)
	{
		/*if message = HotKey then do action*/
		if (msg.message == WM_HOTKEY)
		{
			//Change the current state of program
			if (startGame == true) {
				startGame = false;
			}
			else {
				startGame = true;
			}
		}
	}

	while (true) {
		try {
			//If game has started, let's run
			if (startGame == true) {
				//Instruction function to guide user using this program
				//instruction();

				//Capture Window Desktop to get frame but there is one problem as the frame'resolution 
				//is not high enough
				Mat oriSceneMat = captureScreen();
				//imwrite("C:/Users/lehuy/OneDrive/Pictures/TopIQTotal/Formatted/999.png", oriSceneMat);

				//Read in templates and process scene image
				//dirTemplate: directory of the template folder
				//dirScene: directory of sceneMat
				string dirTemplate = "../TopIQTotal/Selected/";
				string dirScene = "../TopIQTotal/Formatted/999.png";
				/*Test image*/
				Mat testMat = imread(dirScene);

				ImageProcessor imgProcessor(dirTemplate, dirScene);

				//Get string from image
				imgProcessor.takeSceneMatAndReturnString(oriSceneMat);
				//imgProcessor.takeSceneMatAndReturnString(testMat);

				string result;
				result = imgProcessor.result;
				cout << "result: " << result << endl;
				//Verifcation
				int verifed = overAllProcess(result);
				cout << "res: " << verifed << endl;

				//Auto click if result is true
				//(x1,y) is the position to click true
				//(x2,y) is the position to click false
				int x1 = 630, x2 = x1 + 90;
				int y = 201;
				if (verifed == 1) {
					//Generate new position
					Point truePos;
					clickingPos(LOWERPOINTTRUE, UPPERPOINTTRUE, truePos);
					
					//Auto click if result is true
					autoClick(truePos.x, truePos.y);
					cout << "TH1" << endl;

					//Save log in case there is error
					imwrite("../TopIQTotal/Formatted/999.png", oriSceneMat);
				}
				else if (verifed == 0) {
					//Generate new position
					Point falsePos;
					clickingPos(LOWERPOINTFALSE, UPPERPOINTFALSE, falsePos);

					//Auto click if result is false
					autoClick(falsePos.x, falsePos.y);
					cout << "TH2" << endl;

					//Save log in case there is error
					imwrite("../TopIQTotal/Formatted/999.png", oriSceneMat);
				}
				else {
					//Do not click if input image is null
					cout << "TH3" << endl;
				}

				cout << "currentQues: " << currentQues << endl;
				currentQues++;
				//Sleep for the next question
				int upperTime, lowerTime;
				if (currentQues < 60) {
					upperTime = 1500;
					lowerTime = 1400;

					sleepIntervalRan(lowerTime, upperTime, sleepingInterval);
				} 
				else if (currentQues < 140) {
					upperTime = 1300;
					lowerTime = 1200;

					sleepIntervalRan(lowerTime, upperTime, sleepingInterval);
				}
				else {
					upperTime = 1000;
					lowerTime = 900;

					sleepIntervalRan(lowerTime, upperTime, sleepingInterval);
				}
				Sleep(sleepingInterval);
			}
		}
		catch (cv::Exception & e)
		{
			cerr << e.msg << endl; // output exception message
		}
	}

	return 0;
}

/*-----------------------------  GUIDELINES  ---------------------------------*/
void instruction() {
	cout << "This program is used for playing game TopIQ on Android mobile devices " << endl <<
		"The math expression recognition phase depends much on the resolution of displaying devices" << endl <<
		"and this program perform most effectively on the resolution 1028x720" << endl <<
		"If your device resolution is different, such as HD resolution: 1920x1080, you need to change" << endl <<
		"some configuration value stored in file 'configuration.txt'" << endl <<
		"1. Open any image in the folder 'TopIqTotal/Rename' and set full size" << endl <<
		"2. Press ALT+s to start program" << endl << 
		"3. Program auto run and move the mouse to the right answer\n";
}
/*-----------------------------Function details ------------------------------*/

