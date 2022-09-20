#include "Game.h"

Game::Game()
	:screen(),
	tetris(),
	tetrisInfo(),
	state(GAMESTATE::WAIT),
	gameUpdateToken(false),
	speed(1),
	score(0),
	stringNowTime("00:00"),
	nowTime(0),
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

	screen.screenPrintTextInfo(infoBoard);
	screen.screenPrintNextBlock(tetris.getNextBlockBoard());
}

void Game::gameUpdate(KEYCODE key)
{
	
	gameUpdateToken = gameUpdateToken || keyInputProcess(key);

	gameUpdateToken = gameUpdateToken || blockDownReqularInterval();

	gameInfoUpdate();

	checkGameProgress();
	
}

void Game::gameRender()
{
	if (gameUpdateToken)
	{
		tetris.drawGhostPiece();
		screen.screenUpdate(tetris.getMap());
		screen.screenPrintTextInfo(infoBoard);
		screen.screenPrintNextBlock(tetris.getNextBlockBoard());
		gameUpdateToken = false;
	}
}

bool Game::blockDownReqularInterval()
{
	nowTime = clock();
	if (state == GAMESTATE::PLAYING && nowTime - blockDownTime >= 1000 / speed)
	{
		blockDownTime = nowTime;

		return keyInputProcess(KEYCODE::DOWN);
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

	else if (state != GAMESTATE::PAUSE && state != GAMESTATE::WAIT)
	{
		if (key == KEYCODE::UP)
			return tetris.rotateBlockRight();
		else if (key == KEYCODE::DOWN)
			return tetris.moveBlock({ 1,0 });
		else if (key == KEYCODE::LEFT)
			return tetris.moveBlock({ 0,-1 });
		else if (key == KEYCODE::RIGHT)
			return tetris.moveBlock({ 0,1 });
		// input function 
	}

	return false;
}

void Game::checkGameProgress()
{
	nowTime = clock();

	switch (state)
	{
	case GAMESTATE::PLAYING:
		if (tetris.blockTouchBottom())
		{
			int eraseLine = tetris.eraseLine();
			if(eraseLine > 0)
			{
				gameScoreUpdate(eraseLine);
				waitTime = clock();
				state = GAMESTATE::WAIT;
			}
			else
			{
				LockDelayTime = clock();
				state = GAMESTATE::LOCKDELAY;
			}
			
		}
		break;
	case GAMESTATE::LOCKDELAY:
		
		if (!tetris.blockTouchBottom())
		{
			state = GAMESTATE::PLAYING;
		}
		else if (nowTime - LockDelayTime > 500)
		{
			waitTime = clock();
			state = GAMESTATE::WAIT;
			tetris.destroyBlock();
		}
		break;
	case GAMESTATE::WAIT:
		if (nowTime - waitTime > 200)
		{
			tetris.createBlock();
			state = GAMESTATE::PLAYING;
		}
		break;
	}
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
			index--;
			index--;

			if (stringNowTime[index] == '9')
			{
				stringNowTime[index] = '0';
				index--;

				if (stringNowTime[index] == '5')
					stringNowTime[index] = '0';
				else
				{
					stringNowTime[index]++;
				}
			}
			else
			{
				stringNowTime[index]++;
			}
		}
		else
		{
			stringNowTime[index]++;
		}
	}
	else
	{
		stringNowTime[index]++;
	}
}

void Game::gameTimerUpdate()
{
	nowTime = clock();
	if (nowTime - gameRunTime >= 1000)
	{
		gameRunTime = nowTime;
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
	gameTimerUpdate();
	gameScoreUpdate(0);
	gameSpeedUpdate();
	
	infoBoard[7] = { "  시간 : " + stringNowTime };
	infoBoard[9] = {"  점수 : " + to_string(score) };
	infoBoard[11] = {"  속도 : " + to_string(speed) };
}