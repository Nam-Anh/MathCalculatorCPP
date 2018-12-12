#include "../headers/RandomGenerator.h"

void clickingPos(Point lowerPoint, Point upperPoint, Point& clickedPoint) {
	int tmpX, tmpY;

	//Random the x-position 
	tmpX = lowerPoint.x + (std::rand() % (upperPoint.x - lowerPoint.x + 1));
	//Random the y-position 
	tmpY = lowerPoint.y + (std::rand() % (upperPoint.y - lowerPoint.y + 1));

	clickedPoint.x = tmpX;
	clickedPoint.y = tmpY;
}
void sleepIntervalRan(int lowerTime, int upperTime, int& sleepTime) {
	sleepTime = lowerTime + (std::rand() % (upperTime - lowerTime + 1));
}