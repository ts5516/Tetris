#include "Screen.h"

Screen::Screen()
	:screenHandle(),
	screenDW(),
	workBufferIndex(0)
{
	for (int i = 0; i < SCREEN_HANDLE_NUM; i++)
	{
		screenHandle[i] = CreateConsoleScreenBuffer(
			GENERIC_READ | GENERIC_WRITE,
			0, NULL,
			CONSOLE_TEXTMODE_BUFFER, NULL);

		showCursor(false, screenHandle[i]);
	}
}

Screen::~Screen()
{
	for(int i = 0; i< SCREEN_HANDLE_NUM; i++)
		CloseHandle(screenHandle[i]);
}

void Screen::screenInitialize(pair<short, short> val, vector<vector<int>> board)
{
	for (short i = 0; i < board.size(); i++)
	{
		for (short j = 0; j < board[0].size(); j++)
		{
			if (board[i][j] == -1)
			{
				COORD pos = { j * 2+ val.first, i+ val.second };

				printToken(
					workBufferIndex, pos,
					0, WALL);
				printToken(
					1-workBufferIndex, pos,
					0, WALL);
			}
		}
	}
}

void Screen::screenUpdate(pair<short, short> val, vector<vector<int>> board)
{
	for (short i = 0; i < board.size(); i++)
	{
		for (short j = 0; j < board[0].size(); j++)
		{
			COORD pos = { val.first+ j * 2, val.second + i };
			int tile = board[i][j];

			if (tile == 1)
			{
				printToken(
					workBufferIndex, pos,
					8, GHOST);
			}
			else if (tile > 0 || tile < -1)
			{
				printToken(
					workBufferIndex, pos,
					abs(tile), BRICK);
			}
			else if (tile == 0)
			{
				printToken(
					workBufferIndex, pos,
					0, BLANK);
			}
			else if (tile == -1)
			{
				printToken(
					workBufferIndex, pos,
					7, WALL);
			}
		}
	}
}

void Screen::screenFlip()
{
	SetConsoleActiveScreenBuffer(
		screenHandle[workBufferIndex]);

	workBufferIndex = 1 - workBufferIndex;
}


void Screen::screenClear()
{
	FillConsoleOutputCharacter(
		screenHandle[workBufferIndex],
		' ', 80 * 40,
		{ 0,0 }, &screenDW);
}

void Screen::screenPrintTextInfo(
	pair<short, short> val,
	vector<vector<string>> infoBoard)
{
	for (int i = 0; i < infoBoard.size(); i++)
	{
		for (int j = 0; j < infoBoard[0].size(); j++)
		{
			COORD pos = { 
				val.first + j * 2,
				val.second + i };

			printToken(
				workBufferIndex, pos,
				7, infoBoard[i][j]);
			printToken(
				1 - workBufferIndex, pos,
				7, infoBoard[i][j]);
		}
	}
}

void Screen::screenPrintNextBlock(
	pair<short, short> val,
	vector<vector<int>> nextBlockBoard)
{
	for (int i = 0; i < nextBlockBoard.size(); i++)
	{
		for (int j = 0; j < nextBlockBoard[0].size(); j++)
		{
			int tile = nextBlockBoard[i][j];
			COORD pos = { 
				val.first + j * 2 ,
				val.second + i };

			SetConsoleCursorPosition(
				screenHandle[workBufferIndex],
				pos);

			if (tile == -1)
			{
				printToken(
					workBufferIndex, pos,
					7, WALL);
				printToken(
					1 - workBufferIndex, pos,
					7, WALL);
			}
			else if (tile > 0)
			{
				printToken(
					workBufferIndex, pos,
					abs(tile), BRICK);
				printToken(
					1 - workBufferIndex, pos,
					abs(tile), BRICK);
			}
			else if (tile == 0)
			{
				printToken(
					workBufferIndex, pos,
					0, BLANK);
				printToken(
					1 - workBufferIndex, pos,
					0, BLANK);
			}
		}
	}
}

void Screen::printToken(
	int bufferIndex, COORD pos,
	int color, const string token)
{
	if (color > 0)
		SetConsoleTextAttribute(
			screenHandle[bufferIndex],
			color);

	SetConsoleCursorPosition(
		screenHandle[bufferIndex],
		pos);

	WriteFile(
		screenHandle[bufferIndex],
		token.c_str(),
		(DWORD)token.size(),
		&screenDW,
		NULL);
}

void Screen::showCursor(bool showFlag, HANDLE out)
{
	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(
		out,
		&cursorInfo);

	cursorInfo.bVisible = showFlag;
	
	SetConsoleCursorInfo(
		out,
		&cursorInfo);
}