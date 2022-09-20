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
	NOCOLOR = 0,
	RED = 4,
	PURPLE = 5,
	ORANGE = 6,
	BLUE = 9,
	GREEN = 10,
	SKY = 11,
	YELLOW = 14
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

	Point(pair<int, int> point)
	{
		this->Y = point.first;
		this->X = point.second;
	}

	bool operator==(const Point& p) const
	{
		return X == p.X && Y == p.Y;
	}

	Point& operator+=(const Point& p)
	{
		this->Y += p.Y;
		this->X += p.X;

		return *this;
	}

	Point operator+(const Point& p)
	{
		return Point(this->Y + p.Y, this->X + p.X);
	}
};

struct Tetromino
{
	BLOCKTYPE type;
	vector<Point> position;
	vector<Point> rotation;
	vector<Point> ghostpiecePosition;
	int color;

	Tetromino()
		:type(BLOCKTYPE::NONE),
		position({ {0,0},{0,0},{0,0},{0,0} }),
		rotation({ {0,0},{0,0},{0,0},{0,0} }),
		ghostpiecePosition({ {0,0},{0,0},{0,0},{0,0} }),
		color(COLORTYPE::NOCOLOR)
	{	}

	Tetromino(BLOCKTYPE type, vector<Point> position,
		vector<Point> rotation, int color)
		:type(type),
		position(position),
		rotation(rotation),
		ghostpiecePosition({ {0,0},{0,0},{0,0},{0,0} }),
		color(color)
	{	}

	bool operator==(const Tetromino& tet)
	{
		return this->type == tet.type &&
			this->position == tet.position &&
			this->rotation == tet.rotation &&
			this->ghostpiecePosition == tet.ghostpiecePosition &&
			this->color == tet.color;
	}
};


const int BOARD_ROW = 22, BOARD_COL = 12;
const int NEXTBLOCKBOARD_ROW = 4, NEXTBLOCKBOARD_COL = 6;


const int TETROMINO_NUM = 7;
const int TILE_NUM = 4;


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

	
	bool canBlockPutThisPoints(vector<Point> points);
	Tetromino canRotateBlock();

	bool blockTouchBottom();

	bool hardDrop();

	void drawGhostPiece();

	int eraseLine();
	bool eraseOneLine(int line);

	bool moveBlock(const Point& p);
	bool rotateBlockRight();

	void eraseBlockPastTrace();

	void destroyBlock();
	void createBlock();
	void createNextBlock();


	vector<Point> rotatePoints(const vector<Point>& points);
	vector<Point> movePoints(const Point& move, const vector<Point>& points);

	Point getTranslateDistance()
	{
		return { block.position[0].Y - block.rotation[0].Y,
			block.position[0].X - block.rotation[0].X };
	}

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

	const Tetromino I_;
	const Tetromino L_;
	const Tetromino J_;
	const Tetromino O_;
	const Tetromino S_;
	const Tetromino T_;
	const Tetromino Z_;
	const Tetromino tetrominos[TETROMINO_NUM] = { I_, L_, J_, O_, S_, T_, Z_ };

	vector<vector<int>> map;
	vector<vector<int>> nextBlockBoard;

	Tetromino block;
	Tetromino nextBlock;
};

#endif TETRIS_H_


