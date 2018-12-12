#ifndef UTILITY_H
#define UTILITY_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/imgproc.hpp>
#include <vector>
#include <opencv2/core/types.hpp>
#include <opencv2/core/mat.hpp>

using namespace std;
using namespace cv;


/*Count number of white pixels*/
extern int pixNonZero(Mat src1, Mat src2);

#endif /* UTILITY_H */

