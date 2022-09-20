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
	PLAYING = 0,
	PAUSE,
	LOCKDELAY,
	WAIT,
	GAMEOVER
};

class Game
{
public:
	Game();

	void gameInit();
	void gameUpdate(KEYCODE key);
	void gameRender();
	void gameExit();
	void gameRestart();

	bool keyInputProcess(KEYCODE key);

	bool blockDownReqularInterval();

	void checkGameProgress();

	void gameScoreUpdate(int eraseLine);
	void gameTimerUpdate();
	void gameSpeedUpdate();
	void gameInfoUpdate();
	
	void gametimeAdd1();
private:
	Screen screen;
	Tetris tetris;
	TetrisInfo tetrisInfo;


	GAMESTATE state;
	bool gameUpdateToken;

	int speed;
	int score;
	string stringNowTime;


	clock_t nowTime;
	clock_t LockDelayTime;
	clock_t blockDownTime;
	clock_t gameRunTime;
	clock_t waitTime;

	vector<vector<string>> infoBoard;
};
#endif // !GAME_H_
