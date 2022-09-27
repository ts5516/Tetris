#pragma once
#include <iostream>
#include <numeric>
#include <math.h>
#include "Tetris.h"
class CPU : public Tetris 
{
public:
	CPU();
	void initialize();

	void calDestination();
	int moveLeft_untilEnd();

	int getDirection() { return direction; }
	int getRotateCount() { return rotateCount; }
	int getmoveCount() { return moveCount; }
private:
	int maxWeight;
	int rotateCount;
	int moveCount;
	int direction;
};