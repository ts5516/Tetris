#pragma once
#include <iostream>
#include <numeric>
#include <math.h>
#include <algorithm>
#include "Tetris.h"
class CPU : public Tetris 
{
public:
	CPU();
	void initialize();

	void calDestination();
	void moveLeft_untilEnd(int& mc);
	void DoAction();
	vector<vector<int>> makeTable();
private:
	int destinationX;
	vector<pair<int, int>> destination;
	int maxWeight;
	int rotateCount;
	int count;
};