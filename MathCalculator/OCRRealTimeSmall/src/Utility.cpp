#include "../headers/Utility.h"

/*Count number of white pixels*/
int pixNonZero(Mat src1, Mat src2) {
    resize(src1, src1, Size(25, 35));
    resize(src2, src2, Size(25, 35));

    /*Mat res = src2 - src1;
     This way has one disadvantage: negative is casted to zero.
     So if one image is full of white, result is all black which is not what we want
     */

    /*We need to write our own function to cast: negative to 255*/
    /*count: variable to count number of white pixels*/
    Mat res = Mat::zeros(Size(25, 35), CV_8UC1);

//    imshow("pix_1", src1);
//    imshow("pix_2", src2);
    int count = 0;
    for (int y = 0; y < res.rows; y++) {
        for (int x = 0; x < res.cols; x++) {
            int minus = src1.at<uchar>(y, x) - src2.at<uchar>(y, x);
            /*If minus equals 0*/
            if (minus >= 0 && minus <= 10) {
                res.at<uchar>(y, x) = 0;
            } else {
                res.at<uchar>(y, x) = 255;
                count++;
            }
        }
    }
//    imshow("res", res);
//    waitKey();
    return count;
}



