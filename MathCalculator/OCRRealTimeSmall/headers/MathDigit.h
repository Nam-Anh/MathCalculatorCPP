#ifndef MATHDIGIT_H
#define MATHDIGIT_H

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
#include "Utility.h"
using namespace std;
using namespace cv;

/*---------------------------- EXTERNAL CLASS --------------------------------*/
class MathDigit {
public:
    int index;
    string type; //2 types: 'num' and 'sign'
    Mat matrix;
    /*------------------------  CONSTRUCTOR  ---------------------------------*/
    MathDigit();
    MathDigit(int index, string type, Mat matrix);
    /*------------------- COMPARE TWO MATHDIGIT ------------------------------*/
    bool operator==(const MathDigit& digit) const;
};
#endif /* MATHDIGIT_H */