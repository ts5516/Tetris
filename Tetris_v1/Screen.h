#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <cmath>
#ifndef SCREEN_H_
#define SCREEN_H_

using namespace std;

const string WALL = "в╩";		// value is -1
const string BRICK = "бс";		// value is colorValue
const string BLANK = "  ";		// value is 0
const string GHOST = "бр";		// value is 1


const int SCREEN_HANDLE_NUM = 2;


class Screen {
public:
	Screen();
	~Screen();


	void screenInitialize(pair<short, short> val, vector<vector<int>> board);
	void screenUpdate(pair<short, short> val, vector<vector<int>> board);
	void screenFlip();
	void screenClear();


	void screenPrintTextInfo(
		pair<short, short> val,
		vector<vector<string>> infoboard);
	void screenPrintNextBlock(
		pair<short, short> val,
		vector<vector<int>> nextBlockBoard);
	void printToken(
		int bufferIndex, COORD pos,
		int color, const string token);
	void showCursor(bool showFlag, HANDLE out);

private:
	HANDLE screenHandle[SCREEN_HANDLE_NUM]; 
	DWORD screenDW;
	int workBufferIndex;
};

#endif