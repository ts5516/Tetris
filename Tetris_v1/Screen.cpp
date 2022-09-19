#include "Screen.h"

Screen::Screen()
	:screenHandle(),
	screenDW()
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

void Screen::screenInitialize(vector<vector<int>> board)
{
	for (int i = 0; i < board.size(); i++)
	{
		for (int j = 0; j < board[0].size(); j++)
		{
			if (board[i][j] == -1)
			{
				COORD pos = { (short)j * 2,(short)i };
				printToken(workBufferIndex, pos, 0, WALL);
				printToken(1-workBufferIndex, pos, 0, WALL);
			}
		}
	}
}

void Screen::screenRender(vector<vector<int>> board)
{
	
}

void Screen::screenFlip()
{
	SetConsoleActiveScreenBuffer(
		screenHandle[workBufferIndex]);
	workBufferIndex = 1 - workBufferIndex; // 0, 1 flip
}

void Screen::screenUpdate(vector<vector<int>> board)
{
	for (int i = 0; i < board.size(); i++)
	{
		for (int j = 0; j < board[0].size(); j++)
		{
			COORD pos = { (short)j * 2,(short)i };
			int tile = board[i][j];
			if (tile == 1)
			{
				printToken(workBufferIndex, pos, 8, GHOST);
			}
			else if (tile > 0 || tile < -1)
			{
				printToken(workBufferIndex, pos, abs(tile), BRICK);
			}
			else if(tile == 0)
			{
				printToken(workBufferIndex, pos, 0, BLANK);
			}
		}
	}

	screenFlip();
}

void Screen::showCursor(bool showFlag, HANDLE out)
{
	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}

void Screen::screenClean(vector<vector<int>> board)
{
	for (int i = 0; i < board.size(); i++)
	{
		for (int j = 0; j < board[0].size(); j++)
		{
			COORD pos = { (short)j * 2,(short)i };
			printToken(workBufferIndex, pos, 0, BLANK);
		}
	}
}

void Screen::screenPrintTextInfo(vector<vector<string>> infoBoard)
{
	for (int i = 0; i < infoBoard.size(); i++)
	{
		for (int j = 0; j < infoBoard[0].size(); j++)
		{
			short xpos = 25 + j*2;
			short ypos = 1 + i;
			COORD pos = { xpos, ypos };
			printToken(workBufferIndex, pos, 7, infoBoard[i][j]);
			printToken(1 - workBufferIndex, pos, 7, infoBoard[i][j]);
		}
	}
}

void Screen::screenPrintNextBlock(vector<vector<int>> nextBlockBoard)
{
	for (int i = 0; i < nextBlockBoard.size(); i++)
	{
		for (int j = 0; j < nextBlockBoard[0].size(); j++)
		{
			short xpos = j*2 + 26;
			short ypos = i + 3;
			COORD pos = { xpos,ypos };
			int tile = nextBlockBoard[i][j];
			SetConsoleCursorPosition(
				screenHandle[workBufferIndex], pos);
			if (tile == -1)
			{
				printToken(workBufferIndex, pos, 7, WALL);
				printToken(1 - workBufferIndex, pos, 7, WALL);
			}
			if (tile > 0)
			{
				printToken(workBufferIndex, pos, abs(tile), BRICK);
				printToken(1 - workBufferIndex, pos, abs(tile), BRICK);
			}
			else if (tile == 0)
			{
				printToken(workBufferIndex, pos, 0, BLANK);
				printToken(1 - workBufferIndex, pos, 0, BLANK);
			}
		}
	}
}

void Screen::printToken(int bufferIndex, COORD pos, int color, const string token)
{
	if (color > 0)
		SetConsoleTextAttribute(screenHandle[bufferIndex], color);

	SetConsoleCursorPosition(screenHandle[bufferIndex], pos);
	WriteFile(screenHandle[bufferIndex], token.c_str(),
		(DWORD)token.size(), &screenDW, NULL);
}