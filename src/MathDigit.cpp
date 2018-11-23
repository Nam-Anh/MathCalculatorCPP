#include "../headers/MathDigit.h"

MathDigit::MathDigit(){
    this->index = -1;
    this->type = "null";
    this->matrix = Mat::zeros(0,0, CV_8UC3);
}
MathDigit::MathDigit(int index, string type, Mat matrix) {
    this->index = index;
    this->type = type;
    this->matrix = matrix;
}

bool MathDigit::operator==(const MathDigit& digit) const {
    if ((this->index == digit.index) && (this->type.compare(digit.type) == 0) && pixNonZero(this->matrix, digit.matrix)) {
        return true;
    }
    return false;
}
