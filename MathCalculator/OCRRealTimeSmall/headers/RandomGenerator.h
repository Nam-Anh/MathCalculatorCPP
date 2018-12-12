#ifndef RANDOMGENERATOR_H
#define RANDOMGENERATOR_H
#include <iostream>
#include <conio.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/core/mat.hpp>

using namespace std;
using namespace cv;

/*Generate the clicking position depending on the TRUE or FALSE*/
extern void clickingPos(Point lowerPoint, Point upperPoint, Point& clickedPoint);
extern void sleepIntervalRan(int lowerTime, int upperTime, int& sleepTime);
#endif /* RANDOMGENERATOR_H */