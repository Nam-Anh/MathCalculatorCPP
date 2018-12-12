#ifndef WINDOWCONTROLLER_H
#define WINDOWCONTROLLER_H
#include "opencv2/highgui.hpp"
#include <Windows.h>
#include <iostream>

using namespace std;
using namespace cv;

/*Convert on windowDesktop object to OpenCV Mat*/
extern Mat hwnd2mat(HWND hwnd);
/*Capture screen and convert to mat*/
extern Mat captureScreen();

#endif /* WINDOWCONTROLLER_H */