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

struct OBJECT_ELEMENT {
	Tetris object;

	GAMESTATE state;
	bool gameUpdateToken;

	clock_t LockDelayTime;
	clock_t blockDownTime;
	clock_t gameRunTime;
	clock_t waitTime;

	TetrisInfo tetrisInfo;
	int speed;
	int score;

	string stringNowTime;
	vector<vector<string>> infoBoard;
};

class Game
{
public:
	Game();

	void init();
	void initOE(OBJECT_ELEMENT& one);
	void update(KEYCODE key);
	void render();

	KEYCODE getInput()
	{
		return input.getInput();
	}
	GAMESTATE getGamestate()
	{
		return player.state;
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
	Input input;

	OBJECT_ELEMENT player;
	OBJECT_ELEMENT cpu;

	const int waitSecond = 200;
	const int oneSecond = 1000;
	const int lockdealySecond = 500;
};
#endif // !GAME_H_