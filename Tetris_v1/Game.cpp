#include "Game.h"

Game::Game()
	:screen(),
	tetris(),
	tetrisInfo(),
	input(0),
	state(GAMESTATE::WAIT),
	speed(3),
	score(0),
	stringNowTime("00:00"),
	nowTime(0),
	renderTime(clock()),
	gameRunTime(clock()),
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

	tetris.createBlock();
}

void Game::gameUpdate(int input)
{
	nowTime = clock();
	bool updateToken = false;
	if (nowTime - gameRunTime >= 1000)
	{
		gameTimerUpdate(1);
		gameRunTime = nowTime;
		updateToken = true;
	}

	if (input == 0)
	{
		if (state == GAMESTATE::PAUSE)
			state == GAMESTATE::PLAYING;
		else if (state == GAMESTATE::PLAYING)
			state == GAMESTATE::PAUSE;
	}

	if (state != GAMESTATE::PAUSE)
	{
		if (input > 0)
		{
			if (input == 72 || input == 97 || input == 120)
				updateToken = updateToken || tetris.rotationBlock();
			else if (input == 80 || input == 115)
				updateToken = updateToken || tetris.moveBlock(MOVE::DOWN);
			else if (input == 75 || input == 100)
				updateToken = updateToken || tetris.moveBlock(MOVE::LEFT);
			else if (input == 77 || input == 119)
				updateToken = updateToken || tetris.moveBlock(MOVE::RIGHT);
			// input function 
		}

		updateToken = updateToken || gameInfoUpdate();
	}

	if (updateToken)
	{
		tetris.drawGhostPiece();
		screen.screenUpdate(tetris.getMap());
		screen.screenPrintTextInfo(infoBoard);
		screen.screenPrintNextBlock(tetris.getNextBlockBoard());
		updateToken = false;
	}
	
}

void Game::gameRender()
{
	nowTime = clock();
	if (nowTime - renderTime >= 1000 / speed)
	{
		renderTime = nowTime;
		if (state == GAMESTATE::PLAYING)
		{
			gameUpdate(80);
		}

		else if (state == GAMESTATE::LOCKDELAY)
		{
			state = GAMESTATE::WAIT;

			if (tetris.blockTouchBottom())
			{
				tetris.destroyBlock();
				return;
			}
		}

		else if (state == GAMESTATE::WAIT)
		{
			tetris.createBlock();
			state = GAMESTATE::PLAYING;
			gameUpdate(0);
			return;
		}
		// render function

		if (tetris.blockTouchBottom())
			state = GAMESTATE::LOCKDELAY;
		else
			state = GAMESTATE::PLAYING;

		
	}
}


void Game::gameTimerUpdate(int second)
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

bool Game::gameInfoUpdate()
{
	bool updateToken = false;


	int getScore = tetrisInfo.getScore(tetris.eraseLine());

	if (getScore > 0)
	{
		score += getScore;
		updateToken = true;
	}
		

	if (tetrisInfo.goalScore(speed) <= score)
	{
		speed++;
		updateToken = true;
	}

	infoBoard[7] = { "  시간 : " + stringNowTime };
	infoBoard[9] = {"  점수 : " + to_string(score) };
	infoBoard[11] = {"  속도 : " + to_string(speed) };

	return updateToken;
}