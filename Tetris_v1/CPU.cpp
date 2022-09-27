#include "CPU.h"

CPU::CPU()
{
	initialize();
}

void CPU::calDestination()
{
	initialize();
	vector<int> emptyArr(getMap()[0].size(), 0);
	vector<vector<int>> table(getMap().size(), emptyArr);
	eraseBlockOnMap();
	vector<vector<int>> temp = getMap();
	for (int i = 1; i < temp[0].size(); i++) {
		bool findBlock = false;
		for (int j = 1; j < temp.size()-1; j++) {
			if (temp[j][i] != -1 && temp[j][i] != 0 && temp[j][i] != 1) {
				findBlock = true;
				table[j][i] = j;
			}
			else if (temp[j][i] == 0 && findBlock) {
				table[j][i] = -j;
			}
		}
	}
	putBlockOnMap();
	int tempMoveCount = moveLeft_untilEnd();
	int tempMoveCount2 = 0;
	for (int k = 0; k < 4; k++) {
		moveLeft_untilEnd();
		tempMoveCount2 = 0;
		while (1) {
			vector<vector<int>> tempTable = table;
			for (int i = 0; i < 4; i++) {
				int ghostX = getBlock().ghostPiecePos[i].X;
				int ghostY = getBlock().ghostPiecePos[i].Y;
				for (int j = ghostY; j < temp.size() - 1; j++) {
					if (tempTable[j][ghostX] == 0) {
						tempTable[j][ghostX] = -j;
					}
				}
				tempTable[ghostY][ghostX] = ghostY;
			}
			int tempResult = 0;
			for (int i = 1; i < tempTable.size() - 1; i++) {
				for (int j = 1; j < tempTable[i].size() - 1; j++) {
					tempResult += tempTable[i][j];
				}
			}
			if (tempResult > maxWeight) {
				maxWeight = tempResult;
				if (k == 3) rotateCount = 0;
				else rotateCount = k;
				direction = (tempMoveCount + tempMoveCount2) / abs((tempMoveCount + tempMoveCount2));
				moveCount = abs(tempMoveCount + tempMoveCount2);
			}
			if (!moveBlock({ 0,1 })) {
				break;
			}
			tempMoveCount2++;
		}
		rotateBlockRight();
	}
	for (int i = 0; i < tempMoveCount2 + tempMoveCount; i++) {
		moveBlock({ 0,-1 });
	}
}

void CPU::initialize()
{
	rotateCount = 0;
	moveCount = 0;
	direction = 0;
	maxWeight = INT_MIN;
}

int CPU::moveLeft_untilEnd()
{
	int tempMoveCount = 0;
	while (1) {
		if (!moveBlock({ 0,-1 })) {
			return tempMoveCount;
			break;
		}
		tempMoveCount--;
	}
}
