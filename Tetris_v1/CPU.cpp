#include "CPU.h"

CPU::CPU()
{
	initialize();
}

void CPU::calDestination()
{
	initialize();
	vector<int> emptyArr(getMap()[0].size(), 0);
	vector<vector<int>> table = makeTable();
	int tempMoveCount = 0;
	moveLeft_untilEnd(tempMoveCount);
	for (int k = 0; k < 4; k++) {
		moveLeft_untilEnd(tempMoveCount);
		while (1) {
			vector<vector<int>> tempTable = table;
			vector<pair<int, int>> tempDestination;
			for (int i = 0; i < 4; i++) {
				int ghostX = getBlock().ghostPiecePos[i].X;
				int ghostY = getBlock().ghostPiecePos[i].Y;
				for (int j = ghostY; j < tempTable.size() - 1; j++) {
					if (tempTable[j][ghostX] == 0) {
						tempTable[j][ghostX] = -j;
					}
				}
				tempTable[ghostY][ghostX] = ghostY;
				tempDestination.push_back({ ghostY,ghostX });
			}

			int result = calWeightSum(tempTable, tempDestination);

			if (result > maxWeight) {
				maxWeight = result;
				rotateCount = k;
				destinationX = tempDestination.front().second;
			}
			tempDestination.clear();

			if (!moveBlock({ 0,1 })) 
				break;
			else
				tempMoveCount++;
		}
		while (tempMoveCount != 0) {
			if (moveBlock({ 0,-1 })) 
				tempMoveCount--;
		}
		rotateBlockRight();
	}
}

void CPU::initialize()
{
	rotateCount = 0;
	destinationX = 0;
	maxWeight = INT_MIN;
}

void CPU::moveLeft_untilEnd(int& mc)
{
	while (1) {
		if (!moveBlock({ 0,-1 })) 
			break;
		else 
			mc--;
	}
}

void CPU::DoAction()
{
	if (rotateCount > 0) {
		rotateBlockRight();
		rotateCount--;
	}
	else {
		int ghostX = getBlock().ghostPiecePos[0].X;
		if (ghostX == destinationX) {
			hardDrop();
		}
		else {
			int direction = (destinationX - ghostX) / abs(destinationX - ghostX);
			moveBlock({ 0,direction });
		}
	}
}

vector<vector<int>> CPU::makeTable()
{
	vector<int> emptyArr(getMap()[0].size(), 0);
	vector<vector<int>> table(getMap().size(), emptyArr);
	eraseBlockOnMap();
	vector<vector<int>> temp = getMap();
	for (int i = 1; i < temp[0].size() - 1; i++) {
		bool findBlock = false;
		for (int j = 1; j < temp.size() - 1; j++) {
			if (temp[j][i] != -1 && temp[j][i] != 0 && temp[j][i] != 1) {
				findBlock = true;
				table[j][i] = j;
			}
			else if (temp[j][i] == 0 && findBlock) {
				table[j][i] = -j;
			}
		}
	}
	for (int i = 1; i < temp.size(); i++) {
		table[i][0] = i;
		table[i][11] = i;
	}
	table[21].assign(12,21);
	putBlockOnMap();
	return table;
}

int CPU::calWeightSum(vector<vector<int>> table ,vector<pair<int, int>> arr)
{
	int result = 0;
	int dx[3] = { 0,1,-1 };
	int dy[3] = { 1,0,0 };

	for (int i = 0; i < 4; i++) {
		int ghostX = getBlock().ghostPiecePos[i].X;
		int ghostY = getBlock().ghostPiecePos[i].Y;
		for (int j = 0; j < 3; j++) {
			pair<int, int> tempPos = { ghostY + dy[j],ghostX + dx[j] };
			auto iter = find_if(arr.begin(), arr.end(), [tempPos](const pair<int, int>& a) {
				if (tempPos.first == a.first && tempPos.second == a.second)
					return true;
				return false;
				});
			if (iter == arr.end()) {
				if (tempPos.first >= 0 && tempPos.second >= 0)
					result += table[tempPos.first][tempPos.second];
			}
		}
	}

	return result;
}

void CPU::createBlock() {
	if (existBlock(*getBlockAddress()))
		return;

	else if (!existBlock(*getNextBlockAddress()))
	{
		createNextBlock();
	}

	*getBlockAddress() = *getNextBlockAddress();
	createNextBlock();

	for (int i = 0; i < TILE_NUM; i++)
	{
		getBlockAddress()->position[i].X += BOARD_COL / 2 - 1;
		getBlockAddress()->position[i].Y += 1;
	}

	putBlockOnMap();
	calDestination();
}