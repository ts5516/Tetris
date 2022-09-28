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

	cpu.tetris.calDestination();
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


void Game::update(KEYCODE key)
{
	
	player.gameUpdateToken = keyInputProcess(key);

	gameInfoUpdate();

	int checkBlockPlacePlayer = player.tetris.blockTouchBottom();
	int checkBlockPlaceCPU = cpu.tetris.blockTouchBottom();
	switch (player.state)
	{
	case GAMESTATE::PLAYING:

		if (checkBlockPlacePlayer != -1)
		{
			if (checkBlockPlacePlayer > 0 || key==KEYCODE::SPACE)
			{
				gameScoreUpdate(player,checkBlockPlacePlayer);
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
			player.gameUpdateToken = keyInputProcess(KEYCODE::DOWN);
		}

		break;
	case GAMESTATE::LOCKDELAY:

		if (checkBlockPlacePlayer < 0)
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
			player.tetris.lockBlock();
			
			if (player.tetris.toppedOut())
			{
				player.state = GAMESTATE::GAMEOVER;
				player.gameUpdateToken = true;
				player.tetris.boardReset();
				gameInfoUpdate();
				
			}
			else
			{
				player.tetris.createBlock();
				player.state = GAMESTATE::PLAYING;
				player.blockDownTime = getNowTime();
			}
		}

		break;
	}

	if (getNowTime() - cpuExcuteTime >= 200) {
		cpu.tetris.DoAction();
		cpu.gameUpdateToken = true;
		cpuExcuteTime = getNowTime();
	}

	switch (cpu.state)
	{
	case GAMESTATE::PLAYING:

		if (checkBlockPlaceCPU != -1)
		{
			if (checkBlockPlaceCPU > 0 || key == KEYCODE::SPACE)
			{
				gameScoreUpdate(cpu,checkBlockPlaceCPU);
				cpu.waitTime = clock();
				cpu.state = GAMESTATE::WAIT;
			}
			else
			{
				cpu.LockDelayTime = clock();
				cpu.state = GAMESTATE::LOCKDELAY;
			}

		}
		else if (getNowTime() - cpu.blockDownTime > oneSecond / cpu.speed)
		{
			cpu.blockDownTime = getNowTime();
			cpu.gameUpdateToken = cpu.tetris.moveBlock({ 1,0 });
		}

		break;
	case GAMESTATE::LOCKDELAY:

		if (checkBlockPlaceCPU < 0)
		{
			cpu.state = GAMESTATE::PLAYING;
		}
		else if (getNowTime() - cpu.LockDelayTime > lockdealySecond)
		{
			cpu.LockDelayTime = getNowTime();
			cpu.state = GAMESTATE::WAIT;

		}

		break;
	case GAMESTATE::WAIT:

		if (getNowTime() - cpu.waitTime > waitSecond)
		{
			cpu.waitTime = getNowTime();
			cpu.tetris.lockBlock();

			if (cpu.tetris.toppedOut())
			{
				cpu.state = GAMESTATE::GAMEOVER;
				cpu.tetris.boardReset();
				cpu.gameUpdateToken = true;
				gameInfoUpdate();

			}
			else
			{
				cpu.tetris.createBlock();
				cpu.tetris.calDestination();
				cpu.state = GAMESTATE::PLAYING;
				cpu.blockDownTime = getNowTime();
			}
		}

		break;
	}
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

void Game::gameInfoUpdate()
{
	if (player.state == GAMESTATE::GAMEOVER)
	{
		player.infoBoard[0] = { "게임오버되었습니다" };
	}
	else
	{
		gameTimerUpdate(player);
		gameSpeedUpdate(player);

		player.infoBoard[7] = { "  시간 : " + player.stringNowTime };
		player.infoBoard[9] = { "  점수 : " + to_string(player.score) };
		player.infoBoard[11] = { "  속도 : " + to_string(player.speed) };
	}
	if (cpu.state == GAMESTATE::GAMEOVER)
	{
		cpu.infoBoard[0] = { "게임오버되었습니다" };
	}
	else
	{
		gameTimerUpdate(cpu);
		gameSpeedUpdate(cpu);
		cpu.infoBoard[7] = { "  시간 : " + cpu.stringNowTime };
		cpu.infoBoard[9] = { "  점수 : " + to_string(cpu.score) };
		cpu.infoBoard[11] = { "  속도 : " + to_string(cpu.speed) };
	}
}