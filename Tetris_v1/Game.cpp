#include "Game.h"

Game::Game()
	:screen(),
	input()
{
	initOE<Tetris>(player);
	initOE<CPU>(cpu);
	init();
	cpuExcuteTime = clock();
}

void Game::init()
{
	screen.screenInitialize({0,0}, player.tetris.getMap());
	screen.screenInitialize({50,0}, cpu.tetris.getMap());
	screen.screenPrintTextInfo(
		{ 25,1 },
		player.infoBoard);
	screen.screenPrintTextInfo(
		{ 76,1 },
		cpu.infoBoard);
	screen.screenPrintNextBlock(
		{ 26,3 },
		player.tetris.getNextBlockBoard());
	screen.screenPrintNextBlock(
		{ 77,3 },
		cpu.tetris.getNextBlockBoard());
	player.tetris.createBlock();
	cpu.tetris.createBlock();
}

template<typename T>
void Game::initOE(OBJECT_ELEMENT<T>& oe)
{
	oe.state = GAMESTATE::PLAYING;
	oe.gameUpdateToken = true;

	oe.LockDelayTime = clock();
	oe.blockDownTime = clock();
	oe.gameRunTime = clock();
	oe.waitTime = clock();

	oe.speed = 1;
	oe.score = 0;

	oe.stringNowTime = "00:00";

	oe.infoBoard.push_back({ "  다음 블록  " });
	oe.infoBoard.push_back({ "" });
	oe.infoBoard.push_back({ "" });
	oe.infoBoard.push_back({ "" });
	oe.infoBoard.push_back({ "" });
	oe.infoBoard.push_back({ "" });
	oe.infoBoard.push_back({ "" });
	oe.infoBoard.push_back({ "  시간 : " + oe.stringNowTime });
	oe.infoBoard.push_back({ "" });
	oe.infoBoard.push_back({ "  점수 : " + to_string(oe.score) });
	oe.infoBoard.push_back({ "" });
	oe.infoBoard.push_back({ "  속도 : " + to_string(oe.speed) });
}

template<typename T>
void Game::updateTetris(OBJECT_ELEMENT<T>& oe, KEYCODE key) {
	int checkBlockPlace = oe.tetris.blockTouchBottom();

	switch (oe.state)
	{
	case GAMESTATE::PLAYING:
		if (checkBlockPlace != -1)
		{
			if (checkBlockPlace > 0 || key == KEYCODE::SPACE)
			{
				gameScoreUpdate(oe, checkBlockPlace);
				oe.waitTime = clock();
				oe.state = GAMESTATE::WAIT;
			}
			else
			{
				oe.LockDelayTime = clock();
				oe.state = GAMESTATE::LOCKDELAY;
			}

		}
		else if (getNowTime() - oe.blockDownTime > oneSecond / oe.speed)
		{
			oe.blockDownTime = getNowTime();
			oe.gameUpdateToken = oe.tetris.moveBlock({ 1,0 });
		}
		break;
	case GAMESTATE::LOCKDELAY:
		if (checkBlockPlace < 0)
		{
			oe.state = GAMESTATE::PLAYING;
		}
		else if (getNowTime() - oe.LockDelayTime > lockdealySecond)
		{
			oe.LockDelayTime = getNowTime();
			oe.state = GAMESTATE::WAIT;

		}
		break;
	case GAMESTATE::WAIT:
		if (getNowTime() - oe.waitTime > waitSecond)
		{
			oe.waitTime = getNowTime();
			oe.tetris.lockBlock();

			if (oe.tetris.toppedOut())
			{
				oe.state = GAMESTATE::GAMEOVER;
				oe.gameUpdateToken = true;
				oe.tetris.boardReset();
			}
			else
			{
				oe.tetris.createBlock();
				oe.state = GAMESTATE::PLAYING;
				oe.blockDownTime = getNowTime();
			}
		}
		break;
	}
}

void Game::update(KEYCODE key)
{
	
	player.gameUpdateToken = keyInputProcess(key);

	if (getNowTime() - cpuExcuteTime >= 200) {
		cpu.tetris.DoAction();
		cpu.gameUpdateToken = true;
		cpuExcuteTime = getNowTime();
	}

	gameInfoUpdate(player);
	gameInfoUpdate(cpu);
	updateTetris(player, key);
	updateTetris(cpu, key);
}

void Game::render()
{
	screen.screenClear();
	if (player.gameUpdateToken || cpu.gameUpdateToken)
	{
		if (player.state == GAMESTATE::GAMEOVER)
		{
			screen.screenUpdate({ 0,0 },player.tetris.getMap());
			screen.screenPrintTextInfo(
				{ 3,3 }, player.infoBoard);
		}
		else
		{
			screen.screenPrintTextInfo(
				{ 25,1 }, player.infoBoard);
			screen.screenPrintNextBlock(
				{ 26,3 }, player.tetris.getNextBlockBoard());
			screen.screenUpdate({ 0,0 }, player.tetris.getMap());
		}
		if (cpu.state == GAMESTATE::GAMEOVER)
		{
			screen.screenUpdate({ 50,0 },cpu.tetris.getMap());
			screen.screenPrintTextInfo(
				{ 53,3 }, cpu.infoBoard);
		}
		else
		{
			screen.screenPrintTextInfo(
				{ 76,1 }, cpu.infoBoard);
			screen.screenPrintNextBlock(
				{ 77,3 }, cpu.tetris.getNextBlockBoard());
			screen.screenUpdate({ 50,0 }, cpu.tetris.getMap());
		}
		screen.screenFlip();
		player.gameUpdateToken = false;
		cpu.gameUpdateToken = false;
	}
}

bool Game::keyInputProcess(KEYCODE key)
{
	if (key == KEYCODE::ESC)
	{
		if (player.state == GAMESTATE::PAUSE)
			player.state = GAMESTATE::PLAYING;
		else if (player.state == GAMESTATE::PLAYING)
			player.state = GAMESTATE::PAUSE;
	}

	else if (player.state == GAMESTATE::PLAYING ||
		player.state == GAMESTATE::LOCKDELAY)
	{
		if (key == KEYCODE::UP)
			return player.tetris.rotateBlockRight();
		else if (key == KEYCODE::DOWN)
			return player.tetris.moveBlock({ 1,0 });
		else if (key == KEYCODE::LEFT)
			return player.tetris.moveBlock({ 0,-1 });
		else if (key == KEYCODE::RIGHT)
			return player.tetris.moveBlock({ 0,1 });
		else if (key == KEYCODE::SPACE)
			return player.tetris.hardDrop();
	}

	else if (player.state == GAMESTATE::GAMEOVER)
	{
		if (key == KEYCODE::ENTER)
		{
			player.state = GAMESTATE::END;
			return true;
		}
	}
	
	return false;
}

void Game::gametimeAdd1(string& str)
{
	int index = str.size() - 1;
	if (str[index] == '9')
	{
		str[index] = '0';
		index--;
		
		if (str[index] == '5')
		{
			str[index] = '0';
			index--; index--;

			if (str[index] == '9')
			{
				str[index] = '0';
				index--;

				if (str[index] == '5')
					str[index] = '0';
				else
					str[index]++;
			}
			else
				str[index]++;
		}
		else
			str[index]++;
	}
	else
		str[index]++;
}

template<typename T>
void Game::gameTimerUpdate(OBJECT_ELEMENT<T>& oe)
{
	if (getNowTime() - oe.gameRunTime > oneSecond)
	{
		oe.gameRunTime = getNowTime();
		gametimeAdd1(oe.stringNowTime);
		oe.gameUpdateToken = true;
	}
}
template<typename T>
void Game::gameScoreUpdate(OBJECT_ELEMENT<T>& oe,int eraseLine)
{
	int getScore = oe.tetrisInfo.getScore(eraseLine);
	if (getScore > 0)
	{
		oe.score += getScore;
		oe.gameUpdateToken = true;
	}
}

template<typename T>
void Game::gameSpeedUpdate(OBJECT_ELEMENT<T>& oe)
{
	if (oe.tetrisInfo.goalScore(oe.speed) <= oe.score)
	{
		oe.speed++;
		oe.gameUpdateToken = true;
	}
}

template<typename T>
void Game::gameInfoUpdate(OBJECT_ELEMENT<T>& oe)
{
	if (oe.state == GAMESTATE::GAMEOVER)
	{
		oe.infoBoard[0] = { "게임오버되었습니다" };
	}
	else
	{
		gameTimerUpdate(oe);
		gameSpeedUpdate(oe);

		oe.infoBoard[7] = { "  시간 : " + oe.stringNowTime };
		oe.infoBoard[9] = { "  점수 : " + to_string(oe.score) };
		oe.infoBoard[11] = { "  속도 : " + to_string(oe.speed) };
	}
}