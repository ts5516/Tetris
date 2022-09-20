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

	void screenInitialize(vector<vector<int>> board);
	void screenUpdate(vector<vector<int>> board);
	void screenFlip();
	void screenClean(vector<vector<int>> board);


	void screenPrintTextInfo(vector<vector<string>> infoboard);
	void screenPrintNextBlock(vector<vector<int>> nextBlockBoard);

	void showCursor(bool showFlag, HANDLE out);


	void printToken(int bufferIndex, COORD pos, int color, const string token);
private:
	// control screen buffer
	HANDLE screenHandle[SCREEN_HANDLE_NUM]; 
	DWORD screenDW;
	int workBufferIndex = 0;
};

#endif