#pragma once
#include <vector>
#include <ctime>
#include "Screen.h"
#include "Tetris.h"
#include "Input.h"

#ifndef GAME_H_
#define GAME_H_

using namespace std;


enum GAMESTATE
{
	PLAYING,
	PAUSE,
	LOCKDELAY,
	WAIT,
	GAMEOVER,
	END,
};

class Game
{
public:
	Game();

	void gameInit();
	void gameUpdate(KEYCODE key);
	void gameRender();

	KEYCODE getInput()
	{
		return input.getInput();
	}

	GAMESTATE getGamestate()
	{
		return state;
	}

private:

	bool keyInputProcess(KEYCODE key);

	void gameScoreUpdate(int eraseLine);
	void gameTimerUpdate();
	void gameSpeedUpdate();
	void gameInfoUpdate();
	void gametimeAdd1();

	clock_t getNowTime() { return clock(); }

private:
	Screen screen;
	Tetris tetris;
	Input input;
	TetrisInfo tetrisInfo;


	GAMESTATE state;
	bool gameUpdateToken;

	int speed;
	int score;
	string stringNowTime;


	clock_t LockDelayTime;
	clock_t blockDownTime;
	clock_t gameRunTime;
	clock_t waitTime;

	vector<vector<string>> infoBoard;

	const int waitSecond = 200;
	const int oneSecond = 1000;
	const int lockdealySecond = 500;
};
#endif // !GAME_H_
