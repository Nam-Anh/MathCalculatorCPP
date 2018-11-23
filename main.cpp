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

using namespace std;
using namespace cv;

#include <Windows.h>


#define HOTKEY1 1

/*----------------------------- GUIDELINES -----------------------------------*/
/*Some initial instructions about running this program*/
void instruction();
/*----------------------------------------------------------------------------*/

int main(int argc, char** argv) {
	/*Register hotKeys so there are two hotKeys */
	RegisterHotKey(NULL, HOTKEY1, MOD_ALT | MOD_NOREPEAT, 0x53); // ALT+s
																 /*Initilize one message to get message from system*/
	MSG msg = { 0 };
	/*Get message return > 0 if message is not null*/
	while (GetMessage(&msg, NULL, 0, 0) != 0)
	{
		/*if message = HotKey then do action*/
		if (msg.message == WM_HOTKEY)
		{
			try {
				//Instruction function to guide user using this program
				//instruction();

				//Capture Window Desktop to get frame but there is one problem as the frame'resolution 
				//is not high enough
				Mat oriSceneMat = captureScreen();
				//imwrite("../Image/Formatted/999.png", oriSceneMat);
				
				//Read in templates and process scene image
				//dirTemplate: directory of the template folder
				//dirScene: directory of sceneMat
				string dirTemplate = "../Image/Selected/";
				string dirScene = "../Image/Formatted/999.png";
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
				bool verifed = overAllProcess(result);
				cout << "res: " << verifed << endl;

				//Auto click if result is true
				//(x1,y) is the position to click true
				//(x2,y) is the position to click false
				int x1 = 630, x2 = x1 + 90;
				int y = 201;
				if (verifed == true) {
					autoClick(x1,y);
					cout << "TH1" << endl;
				}
				else {
					//Auto click if result is false
					autoClick(x2, y);
					cout << "TH2" << endl;
				}
			}
			catch (cv::Exception & e)
			{
				cerr << e.msg << endl; // output exception message
			}
		}
	}

	
	return 0;
}

/*-----------------------------  GUIDELINES  ---------------------------------*/
void instruction() {
	cout << "This is used for linux terminal\n" 
		"To run this program, you should use this command\n"
		"cd build\n"
		"src/run ../../../MATERIAL/OCR_Pic/Selected/ ../../../MATERIAL/OCR_Pic/Formatted/1.png\n"
		"the argv[1] is the location of number and sign template\n"
		"the argv[2] is the location of original frame\n";
}
/*-----------------------------Function details ------------------------------*/

