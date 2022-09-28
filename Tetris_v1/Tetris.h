#pragma once
#include <Windows.h>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <random>
#include "TetrisInfo.h"
#ifndef TETRIS_H_
#define TETRIS_H_
#define BOARD_ROW 22 
#define BOARD_COL 12
#define NEXTBLOCKBOARD_ROW 4
#define NEXTBLOCKBOARD_COL 6
#define TETROMINO_NUM 7
#define TILE_NUM 4
using namespace std;


//-------------------------------------------------------
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
	vector<Point> ghostPiecePos;
	int color;


	Tetromino()
		:type(BLOCKTYPE::NONE),
		position({ {0,0},{0,0},{0,0},{0,0} }),
		rotation({ {0,0},{0,0},{0,0},{0,0} }),
		ghostPiecePos({ {0,0},{0,0},{0,0},{0,0} }),
		color(COLORTYPE::NOCOLOR)
	{	}
	Tetromino(BLOCKTYPE type, vector<Point> position,
		vector<Point> rotation, int color)
		:type(type),
		position(position),
		rotation(rotation),
		ghostPiecePos({ {0,0},{0,0},{0,0},{0,0} }),
		color(color)
	{	}


	bool operator==(const Tetromino& tet)
	{
		return this->type == tet.type &&
			this->position == tet.position &&
			this->rotation == tet.rotation &&
			this->ghostPiecePos == tet.ghostPiecePos &&
			this->color == tet.color;
	}
};

// enum and struct
//--------------------------------------------------------



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
	Tetromino getBlock() { return block; }

	bool moveBlock(const Point& p);
	bool rotateBlockRight();
	bool hardDrop();


	int blockTouchBottom();
	bool toppedOut(); // game over logic
	void boardReset();

	void lockBlock();
	virtual void createBlock();
	
	void putBlockOnMap();
	void eraseBlockOnMap();

	bool existBlock(Tetromino _block)
	{
		return _block.type != BLOCKTYPE::NONE;
	}

	void createNextBlock();

	Tetromino* getBlockAddress() { return &block; }
	Tetromino* getNextBlockAddress() { return &nextBlock; }
private:

	Tetromino getRandomBlock(); // 7-bag generator

	void coloringMap(int color);

	void drawGhostPiece();

	int eraseLine();
	bool eraseOneLine(int line);

	bool canBlockPutThisPoints(vector<Point> points);
	Tetromino canRotateBlock();

	vector<Point> rotatePoints(const vector<Point>& points);
	vector<Point> movePoints(const Point& move, const vector<Point>& points);

	Point getTranslateDistance()
	{
		return { block.position[0].Y - block.rotation[0].Y,
			block.position[0].X - block.rotation[0].X };
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
	const vector<Tetromino> tetrominos 
		= { I_, L_, J_, O_, S_, T_, Z_ };


	vector<Tetromino> _7Bag;

	vector<vector<int>> map;
	vector<vector<int>> nextBlockBoard;

	Tetromino block;
	Tetromino nextBlock;
};

#endif TETRIS_H_


