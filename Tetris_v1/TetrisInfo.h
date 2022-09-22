#pragma once

#include <vector>
#include <cmath>
#ifndef TETRISINFO_H_
#define TETRISINFO_H_


using namespace std;

enum BLOCKTYPE
{
	NONE = -1,
	I,
	L,
	J,
	O,
	S,
	T,
	Z
};

class TetrisInfo
{
public:

	TetrisInfo():prevEarnScoreIndex(0)
	{

	}

	int goalScore(int level)
	{
		int levelSum = 0;
		for (int i = 1; i <= level; i++)
			levelSum += i;
		return levelSum * 500;
	}

	int getScore(int lineclearType)
	{
		if (lineclearType == 0)
			return 0;

		int earnScoreIndex = lineclearType - 1;
		if (earnScoreIndex > 2 && prevEarnScoreIndex > 2)
			earnScoreIndex = 4;		// tetris b2b;

		prevEarnScoreIndex = earnScoreIndex;
		return lineClearType[earnScoreIndex];
	}

	vector<pair<int, int>> getSRSwallKickData(
		BLOCKTYPE type, int rotationState)
	{
		if (type == BLOCKTYPE::I)
			return wallKick_I_SRS[rotationState];
		return wallKick_JLTSZ_SRS[rotationState];
	}

private:

	//-----------------------------------------------------
	// Tetris mode score data
	// line clear score

	
	int prevEarnScoreIndex;

	vector<int> lineClearType =
	{
		100,			// single 
		300,			// double 
		500,			// triple 
		800,			// tetris 
		1200,			// tetris b2b
		100,			// t-spin zero
		300,			// t-spin single
		700,			// t-spin double
		600				// t-spin triple
	};

	//-----------------------------------------------------
	//-----------------------------------------------------
	// wallKick  rotation test data_SRS 
	// 0 : spawn state;
	// 1 : rotating right from spawn
	// 2 : rotating 180 from spawn
	// 3 : rotating left from spawn

	vector<vector<pair<int, int>>> wallKick_JLTSZ_SRS =
	{
		{{0,0}, {-1,0}, {-1,-1}, {0,2},  {-1,2} },		// 0 >> 1;
		{{0,0}, {1,0},  {1,1},   {0,-2}, {1,-2} },		// 1 >> 2;
		{{0,0}, {1,0},  {1,-1},  {0,2},  {1,2}  },		// 2 >> 3;
		{{0,0}, {-1,0}, {-1,1},  {0,-2}, {-1,-2}}		// 3 >> 0;
	};

	vector<vector<pair<int, int>>> wallKick_I_SRS =
	{
		{{0,0}, {-2,0}, {1,0},  {-2,1}, {1,-2} },		// 0 >> 1;
		{{0,0}, {-1,0}, {2,0},  {-1,2}, {2,1}  },		// 1 >> 2;
		{{0,0}, {2,0},  {-1,0}, {2,-1}, {-1,2} },		// 2 >> 3;
		{{0,0}, {1,0},  {-2,0}, {1,2},  {-2,1} }		// 3 >> 0;
	};

	//-----------------------------------------------------
};
#endif // !TETRISINFO_H_

