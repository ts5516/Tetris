#pragma once
#include <vector>
#include <ctime>
#include "Screen.h"
#include "Tetris.h"
#include "Input.h"
#include "CPU.h"
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

template<typename T>
struct OBJECT_ELEMENT {
	T tetris;
	string objectName;

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
	template<typename T>
	void initOE(OBJECT_ELEMENT<T>& oe);
	void init();

	template<typename T>
	void updateTetris(OBJECT_ELEMENT<T>& oe, KEYCODE key);
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
	template<typename T>
	void gameScoreUpdate(OBJECT_ELEMENT<T>& oe,int eraseLine);
	template<typename T>
	void gameTimerUpdate(OBJECT_ELEMENT<T>& oe);
	template<typename T>
	void gameSpeedUpdate(OBJECT_ELEMENT<T>& oe);
	template<typename T>
	void gameInfoUpdate(OBJECT_ELEMENT<T>& oe);
	void gametimeAdd1(string& str);

	clock_t getNowTime() { return clock(); }

private:
	Screen screen;
	Input input;

	OBJECT_ELEMENT<Tetris> player;
	OBJECT_ELEMENT<CPU> cpu;

	clock_t cpuExcuteTime;
	
	vector<string> ResultBoard;
	const int waitSecond = 200;
	const int oneSecond = 1000;
	const int lockdealySecond = 500;
};
#endif // !GAME_H_

