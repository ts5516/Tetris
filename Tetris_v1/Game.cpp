#include "Game.h"

Game::Game()
	:screen(),
	tetris(),
	input(),
	tetrisInfo(),
	state(GAMESTATE::PLAYING),
	gameUpdateToken(false),
	speed(1),
	score(0),
	stringNowTime("00:00"),
	LockDelayTime(clock()),
	blockDownTime(clock()),
	gameRunTime(clock()),
	waitTime(clock()),
	infoBoard()
{	
	gameInit();
}

void Game::gameInit()
{
	screen.screenInitialize(tetris.getMap());

	infoBoard.push_back({ "  다음 블록  " });
	infoBoard.push_back({ "" });
	infoBoard.push_back({ "" });
	infoBoard.push_back({ "" });
	infoBoard.push_back({ "" });
	infoBoard.push_back({ "" });
	infoBoard.push_back({ "" });
	infoBoard.push_back({ "  시간 : " + stringNowTime });
	infoBoard.push_back({ "" });
	infoBoard.push_back({ "  점수 : " + to_string(score) });
	infoBoard.push_back({ "" });
	infoBoard.push_back({ "  속도 : " + to_string(speed)});

	screen.screenPrintTextInfo(
		{ 25,1 },
		infoBoard);
	screen.screenPrintNextBlock(
		{ 5,3 },
		tetris.getNextBlockBoard());

	tetris.createBlock();
}

void Game::gameUpdate(KEYCODE key)
{
	
	gameUpdateToken = keyInputProcess(key);

	gameInfoUpdate();

	int checkBlockPlace = tetris.blockTouchBottom();

	switch (state)
	{
	case GAMESTATE::PLAYING:

		if (checkBlockPlace != -1)
		{
			if (checkBlockPlace > 0 || key==KEYCODE::SPACE)
			{
				gameScoreUpdate(checkBlockPlace);
				waitTime = clock();
				state = GAMESTATE::WAIT;
			}
			else
			{
				LockDelayTime = clock();
				state = GAMESTATE::LOCKDELAY;
			}

	 	}
		else if (getNowTime() - blockDownTime > oneSecond / speed)
		{
			blockDownTime = getNowTime();
			keyInputProcess(KEYCODE::DOWN);
		}

		break;
	case GAMESTATE::LOCKDELAY:

		if (checkBlockPlace < 0)
		{
			state = GAMESTATE::PLAYING;
		}
		else if (getNowTime() - LockDelayTime > lockdealySecond)
		{
			LockDelayTime = getNowTime();
			state = GAMESTATE::WAIT;
			
		}

		break;
	case GAMESTATE::WAIT:

		if (getNowTime() - waitTime > waitSecond)
		{
			waitTime = getNowTime();
			tetris.lockBlock();
			
			if (tetris.toppedOut())
			{
				state = GAMESTATE::GAMEOVER;
				tetris.boardReset();
				gameInfoUpdate();
				
			}
			else
			{
				tetris.createBlock();
				state = GAMESTATE::PLAYING;
				blockDownTime = getNowTime();
			}
		}

		break;
	}
	
}

void Game::gameRender()
{
	if (gameUpdateToken)
	{
		if (state == GAMESTATE::GAMEOVER)
		{
			screen.screenClear();
			screen.screenUpdate(tetris.getMap());
			screen.screenPrintTextInfo(
				{ 3,3 }, infoBoard);
		}
		else
		{
			screen.screenUpdate(tetris.getMap());
			screen.screenPrintTextInfo(
				{ 25,1 }, infoBoard);
			screen.screenPrintNextBlock(
				{ 26,3 }, tetris.getNextBlockBoard());
		}
		
		gameUpdateToken = false;
	}
}

bool Game::keyInputProcess(KEYCODE key)
{
	if (key == KEYCODE::ESC)
	{
		if (state == GAMESTATE::PAUSE)
			state = GAMESTATE::PLAYING;
		else if (state == GAMESTATE::PLAYING)
			state = GAMESTATE::PAUSE;
	}

	else if (state == GAMESTATE::PLAYING ||
		state == GAMESTATE::LOCKDELAY)
	{
		if (key == KEYCODE::UP)
			return tetris.rotateBlockRight();
		else if (key == KEYCODE::DOWN)
			return tetris.moveBlock({ 1,0 });
		else if (key == KEYCODE::LEFT)
			return tetris.moveBlock({ 0,-1 });
		else if (key == KEYCODE::RIGHT)
			return tetris.moveBlock({ 0,1 });
		else if (key == KEYCODE::SPACE)
			return tetris.hardDrop();
	}

	else if (state == GAMESTATE::GAMEOVER)
	{
		if (key == KEYCODE::ENTER)
		{
			state = GAMESTATE::END;
			return true;
		}
	}
	
	return false;
}

void Game::gametimeAdd1()
{
	int index = stringNowTime.size() - 1;
	if (stringNowTime[index] == '9')
	{
		stringNowTime[index] = '0';
		index--;
		
		if (stringNowTime[index] == '5')
		{
			stringNowTime[index] = '0';
			index--; index--;

			if (stringNowTime[index] == '9')
			{
				stringNowTime[index] = '0';
				index--;

				if (stringNowTime[index] == '5')
					stringNowTime[index] = '0';
				else
					stringNowTime[index]++;
			}
			else
				stringNowTime[index]++;
		}
		else
			stringNowTime[index]++;
	}
	else
		stringNowTime[index]++;
}

void Game::gameTimerUpdate()
{
	if (getNowTime() - gameRunTime > oneSecond)
	{
		gameRunTime = getNowTime();
		gametimeAdd1();
		gameUpdateToken = true;
	}
	gameUpdateToken = true;
}

void Game::gameScoreUpdate(int eraseLine)
{
	int getScore = tetrisInfo.getScore(eraseLine);
	if (getScore > 0)
	{
		score += getScore;
		gameUpdateToken = true;
	}
}

void Game::gameSpeedUpdate()
{
	if (tetrisInfo.goalScore(speed) <= score)
	{
		speed++;
		gameUpdateToken = true;
	}
}

void Game::gameInfoUpdate()
{
	if (state == GAMESTATE::GAMEOVER)
	{
		infoBoard[0] = { "게임오버되었습니다" };
	}
	else
	{
		gameTimerUpdate();
		gameScoreUpdate(0);
		gameSpeedUpdate();

		infoBoard[7] = { "  시간 : " + stringNowTime };
		infoBoard[9] = { "  점수 : " + to_string(score) };
		infoBoard[11] = { "  속도 : " + to_string(speed) };
	}
	
}