#include "Game.h"

Game::Game()
	:screen(),
	input()
{
	initOE(player);
	initOE(cpu);
	init();
}

void Game::init()
{
	screen.screenInitialize({0,0}, player.object.getMap());
	screen.screenInitialize({50,0}, cpu.object.getMap());
	screen.screenPrintTextInfo(
		{ 25,1 },
		player.infoBoard);
	screen.screenPrintTextInfo(
		{ 76,1 },
		cpu.infoBoard);
	screen.screenPrintNextBlock(
		{ 5,3 },
		player.object.getNextBlockBoard());
	screen.screenPrintNextBlock(
		{ 76,3 },
		cpu.object.getNextBlockBoard());
	player.object.createBlock();
	cpu.object.createBlock();
}

inline void Game::initOE(OBJECT_ELEMENT& one)
{
	one.state = GAMESTATE::PLAYING;
	one.gameUpdateToken = GAMESTATE::PLAYING;

	one.LockDelayTime = clock();
	one.blockDownTime = clock();
	one.gameRunTime = clock();
	one.waitTime = clock();

	one.speed = 1;
	one.score = 0;

	one.stringNowTime = "00:00";

	one.infoBoard.push_back({ "  다음 블록  " });
	one.infoBoard.push_back({ "" });
	one.infoBoard.push_back({ "" });
	one.infoBoard.push_back({ "" });
	one.infoBoard.push_back({ "" });
	one.infoBoard.push_back({ "" });
	one.infoBoard.push_back({ "" });
	one.infoBoard.push_back({ "  시간 : " + one.stringNowTime });
	one.infoBoard.push_back({ "" });
	one.infoBoard.push_back({ "  점수 : " + to_string(one.score) });
	one.infoBoard.push_back({ "" });
	one.infoBoard.push_back({ "  속도 : " + to_string(one.speed) });
}


void Game::update(KEYCODE key)
{
	
	player.gameUpdateToken = keyInputProcess(key);

	gameInfoUpdate();

	int checkBlockPlace = player.object.blockTouchBottom();

	switch (player.state)
	{
	case GAMESTATE::PLAYING:

		if (checkBlockPlace != -1)
		{
			if (checkBlockPlace > 0 || key==KEYCODE::SPACE)
			{
				gameScoreUpdate(checkBlockPlace);
				player.waitTime = clock();
				player.state = GAMESTATE::WAIT;
			}
			else
			{
				player.LockDelayTime = clock();
				player.state = GAMESTATE::LOCKDELAY;
			}

	 	}
		else if (getNowTime() - player.blockDownTime > oneSecond / player.speed)
		{
			player.blockDownTime = getNowTime();
			keyInputProcess(KEYCODE::DOWN);
		}

		break;
	case GAMESTATE::LOCKDELAY:

		if (checkBlockPlace < 0)
		{
			player.state = GAMESTATE::PLAYING;
		}
		else if (getNowTime() - player.LockDelayTime > lockdealySecond)
		{
			player.LockDelayTime = getNowTime();
			player.state = GAMESTATE::WAIT;
			
		}

		break;
	case GAMESTATE::WAIT:

		if (getNowTime() - player.waitTime > waitSecond)
		{
			player.waitTime = getNowTime();
			player.object.lockBlock();
			
			if (player.object.toppedOut())
			{
				player.state = GAMESTATE::GAMEOVER;
				player.object.boardReset();
				gameInfoUpdate();
				
			}
			else
			{
				player.object.createBlock();
				player.state = GAMESTATE::PLAYING;
				player.blockDownTime = getNowTime();
			}
		}

		break;
	}
	
}

void Game::render()
{
	if (player.gameUpdateToken)
	{
		if (player.state == GAMESTATE::GAMEOVER)
		{
			screen.screenClear();
			screen.screenUpdate(player.object.getMap());
			screen.screenPrintTextInfo(
				{ 3,3 }, player.infoBoard);
		}
		else
		{
			screen.screenUpdate(player.object.getMap());
			screen.screenPrintTextInfo(
				{ 25,1 }, player.infoBoard);
			screen.screenPrintNextBlock(
				{ 26,3 }, player.object.getNextBlockBoard());
		}
		
		player.gameUpdateToken = false;
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
			return player.object.rotateBlockRight();
		else if (key == KEYCODE::DOWN)
			return player.object.moveBlock({ 1,0 });
		else if (key == KEYCODE::LEFT)
			return player.object.moveBlock({ 0,-1 });
		else if (key == KEYCODE::RIGHT)
			return player.object.moveBlock({ 0,1 });
		else if (key == KEYCODE::SPACE)
			return player.object.hardDrop();
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

void Game::gametimeAdd1()
{
	int index = player.stringNowTime.size() - 1;
	if (player.stringNowTime[index] == '9')
	{
		player.stringNowTime[index] = '0';
		index--;
		
		if (player.stringNowTime[index] == '5')
		{
			player.stringNowTime[index] = '0';
			index--; index--;

			if (player.stringNowTime[index] == '9')
			{
				player.stringNowTime[index] = '0';
				index--;

				if (player.stringNowTime[index] == '5')
					player.stringNowTime[index] = '0';
				else
					player.stringNowTime[index]++;
			}
			else
				player.stringNowTime[index]++;
		}
		else
			player.stringNowTime[index]++;
	}
	else
		player.stringNowTime[index]++;
}

void Game::gameTimerUpdate()
{
	if (getNowTime() - player.gameRunTime > oneSecond)
	{
		player.gameRunTime = getNowTime();
		gametimeAdd1();
		player.gameUpdateToken = true;
	}
	player.gameUpdateToken = true;
}

void Game::gameScoreUpdate(int eraseLine)
{
	int getScore = player.tetrisInfo.getScore(eraseLine);
	if (getScore > 0)
	{
		player.score += getScore;
		player.gameUpdateToken = true;
	}
}

void Game::gameSpeedUpdate()
{
	if (player.tetrisInfo.goalScore(player.speed) <= player.score)
	{
		player.speed++;
		player.gameUpdateToken = true;
	}
}

void Game::gameInfoUpdate()
{
	if (player.state == GAMESTATE::GAMEOVER)
	{
		player.infoBoard[0] = { "게임오버되었습니다" };
	}
	else
	{
		gameTimerUpdate();
		gameScoreUpdate(0);
		gameSpeedUpdate();

		player.infoBoard[7] = { "  시간 : " + player.stringNowTime };
		player.infoBoard[9] = { "  점수 : " + to_string(player.score) };
		player.infoBoard[11] = { "  속도 : " + to_string(player.speed) };
	}
	
}