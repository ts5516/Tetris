#pragma once
#include <iostream>
#include "Tetris.h"
class CPU : public Tetris 
{
public:
	CPU();
	pair<int, int> calDestination();
private:
	pair<int, int> destination;
	int rotateCount;
	int MoveCount;
};