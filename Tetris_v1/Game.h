#pragma once
#include <vector>
#include <ctime>
#include "Screen.h"
#include "Tetris.h"
#ifndef GAME_H_
#define GAME_H_

using namespace std;


enum GAMESTATE
{
	PLAYING = 0,
	PAUSE,
	LOCKDELAY,
	WAIT,
	END
};

class Game
{
public:
	Game();

	void gameInit();
	void gameUpdate(int input);
	bool gameInfoUpdate();
	void gameRender();
	void gameExit();
	void gameRestart();

	void gameTimerUpdate(int second);

private:
	Screen screen;
	Tetris tetris;
	TetrisInfo tetrisInfo;
	int input;

	GAMESTATE state;


	int speed;
	int score;
	string stringNowTime;


	clock_t nowTime;
	clock_t renderTime;
	clock_t gameRunTime;

	vector<vector<string>> infoBoard;

};
#endif // !GAME_H_
