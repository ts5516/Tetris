#pragma once
#include <Windows.h>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include "TetrisInfo.h"

#ifndef TETRIS_H_
#define TETRIS_H_

using namespace std;

enum COLORTYPE
{
	RED = 4,
	PURPLE = 5,
	ORANGE = 6,
	BLUE = 9,
	GREEN = 10,
	SKY = 11,
	YELLOW = 14
};

enum MOVE
{
	UP = 0,
	DOWN,
	LEFT,
	RIGHT,
	ROTATE
};

enum TOUCH
{
	NOTOUCH = (0x01<<0),
	BOTTOM = (0x01<<1),
	SIDE = (0x01<<2)
};

struct Point
{
	int Y;
	int X;

	Point()
	{
		this->Y = 0, this->X = 0;
	}
	Point(int Y, int X)
	{
		this->Y = Y;
		this->X = X;
	}

	bool operator==(const Point& p) const
	{
		return X == p.X && Y == p.Y;
	}
};

typedef struct 
{
	BLOCKTYPE type;
	vector<Point> position;
	vector<Point> rotation;
	int color;
} Tetromino;

const int BOARD_ROW = 22, BOARD_COL = 12;
const int NEXTBLOCKBOARD_ROW = 4, NEXTBLOCKBOARD_COL = 6;
const int TETROMINO_NUM = 7;

class Tetris
{
public:
	Tetris();
	~Tetris();

	vector<vector<int>> getMap()
	{
		return map;
	}

	vector<vector<int>> getNextBlockBoard()
	{
		return nextBlockBoard;
	}

	bool existBlock(Tetromino _block)
	{
		return _block.type != BLOCKTYPE::NONE;
	}

	
	bool checkMoveBlock(MOVE move);
	bool checkRotateBlock(pair<int, int>& wallKick);


	int blockTouchWall(MOVE move);
	bool blockTouchBottom();
	bool blockMoveSide(MOVE move);


	void drawGhostPiece();
	void eraseGhostPiece();

	int eraseLine();
	bool eraseOneLine(int line);

	bool moveBlock(MOVE move);
	bool rotationBlock();
	void destroyBlock();
	void createBlock();
	void createNextBlock();


	vector<Point> rotateMatrix(vector<Point> matrix);
	vector<Point> moveMatrix(pair<int, int> move, vector<Point> matrix);

	bool mapIndexBlockCanExist(vector<Point> index);

	Tetromino getBlock()
	{
		if (!existBlock(block))
			createBlock();

		return block;
	}

	Tetromino getNextBlock()
	{
		return nextBlock;
	}

private:
	TetrisInfo tetrisInfo;

	Tetromino I_;
	Tetromino L_;
	Tetromino J_;
	Tetromino O_;
	Tetromino S_;
	Tetromino T_;
	Tetromino Z_;
	Tetromino tetrominos[TETROMINO_NUM] = { I_, L_, J_, O_, S_, T_, Z_ };

	vector<vector<int>> map;
	vector<vector<int>> nextBlockBoard;

	Tetromino block;
	Tetromino nextBlock;
};

#endif TETRIS_H_


