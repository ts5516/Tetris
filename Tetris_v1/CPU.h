#pragma once
#include <iostream>
#include <numeric>
#include <math.h>
#include <algorithm>
#include "Tetris.h"

#ifndef CPU_H_
#define CPU_H_
class CPU : public Tetris 
{
public:
	CPU();
	void initialize();

	void createBlock() override;

	void calDestination();

	void DoAction();
private:
	int calWeightSum(vector<vector<int>> table, vector<pair<int,int>> arr);
	vector<vector<int>> makeTable();
	void moveLeft_untilEnd(int& mc);

	int destinationX;
	int maxWeight;
	int rotateCount;
};

#endif CPU_H_