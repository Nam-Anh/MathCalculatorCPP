#include "../headers/MathExpression.h"

void splitStr(string str, vector<string>& result) {
    int idxOperator = -1, idxEqual = -1;
    string fNum = "", sNum = "", finalNum = "";
    string operatorSign = "", equalSign = "";

    for (int i = 0; i < str.size(); i++) {
        //If not operators or equal
        if (str[i] != '+'
                && str[i] != '-'
                && str[i] != '*'
                && str[i] != '/'
                && str[i] != '=') {
            if (idxOperator == -1) {
                //If not yet meet the first sign
                fNum.push_back(str[i]);
            } else if (idxOperator != -1 && idxEqual == -1) {
                //If meet the operator sign but not yet meet equal sign
                sNum.push_back(str[i]);
            } else {
                //Meet the equal sign
                finalNum.push_back(str[i]);
            }
        } else if (str[i] != '=') {
            //if str character at index i is not equal sign so it is one of four operator signs
            idxOperator = i;
            operatorSign.push_back(str[i]);
        } else {
            //str[i] is equal Sign
            idxEqual = i;
            equalSign.push_back(str[i]);
        }
    }

    result.push_back(fNum);
    result.push_back(operatorSign);
    result.push_back(sNum);
    result.push_back(equalSign);
    result.push_back(finalNum);
}
int eval(string fNum, string sNum, string operatorSign){
    int firstNum = atoi(fNum.c_str());
    int secondNum = atoi(sNum.c_str());
    int result = -999;
    
    if(operatorSign.compare("+") == 0){
        result = firstNum + secondNum;
    } else if(operatorSign.compare("-") == 0){
        result = firstNum - secondNum;
    } else if(operatorSign.compare("*") == 0){
        result = firstNum * secondNum;
    } else if(operatorSign.compare("/") == 0){
		if (firstNum % secondNum != 0) {
			result = 999;
		}
		else {
			result = firstNum / secondNum;
		}
    }
    
    return result;
}
bool comRes(int actualRes, string expectedRes){
    int iExpectedRes = atoi(expectedRes.c_str());
    
    return actualRes==iExpectedRes;
}
bool overAllProcess(string str){
    /*---------------------- STEP 1: SPLIT STRING ----------------------------*/
    vector<string> splitedString;
    splitStr(str, splitedString);
    /*----------------------  STEP 2: EVALUATION  ----------------------------*/
    int res;
    res = eval(splitedString[0],splitedString[2],splitedString[1]);
    /*--------  STEP 3: COMPARE ACTUAL RESULT WITH CALCULATED RESULT  --------*/
    bool comparedRes;
    comparedRes = comRes(res, splitedString[4]);
    
    return comparedRes;
}