#ifndef MATHEXPRESSION_H
#define MATHEXPRESSION_H
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>

using namespace std;

/*Split string and store in one vector*/
extern void splitStr(string str, vector<string>& result);
/*Calculate and return result*/
extern int eval(string fNum, string sNum);
/*Check result*/
extern bool comRes(int actualRes, string expectedRes);
/*Overall processing, receive one string and return true or false*/
extern bool overAllProcess(string str);
#endif /* MATHEXPRESSION_H */

