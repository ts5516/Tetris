#include "Tetris.h"

Tetris::Tetris()
	:map(BOARD_ROW, vector<int>(BOARD_COL)),
	nextBlockBoard(NEXTBLOCKBOARD_ROW, vector<int>(NEXTBLOCKBOARD_COL)),
	I_({BLOCKTYPE::I, 
		{ {0,0},{0,1},{0,2},{0,3} }, 
		{ {1,0},{1,1},{1,2},{1,3} },
		COLORTYPE::SKY }),		// sky color
	L_({BLOCKTYPE::L, 
		{ {1,0},{1,1},{1,2},{0,2} },
		{ {1,0},{1,1},{1,2},{0,2} },
		COLORTYPE::ORANGE }),		// orange
	J_({BLOCKTYPE::J,
		{ {0,0},{1,0},{1,1},{1,2} },
		{ {0,0},{1,0},{1,1},{1,2} },
		COLORTYPE::BLUE }),		// blue
	O_({BLOCKTYPE::O,
		{ {0,0},{0,1},{1,0},{1,1} },
		{ {1,0},{1,1},{2,0},{2,1} },
		COLORTYPE::YELLOW }),		// yellow
	S_({BLOCKTYPE::S,
		{ {1,0},{1,1},{0,1},{0,2} },
		{ {1,0},{1,1},{0,1},{0,2} },
		COLORTYPE::GREEN }),		// green
	T_({BLOCKTYPE::T,
		{ {1,0},{1,1},{0,1},{1,2} },
		{ {1,0},{1,1},{0,1},{1,2} },
		COLORTYPE::PURPLE }),		// purple
	Z_({BLOCKTYPE::Z,
		{ {0,0},{0,1},{1,1},{1,2} },
		{ {0,0},{0,1},{1,1},{1,2} },
		COLORTYPE::RED }),		// red
	block(),
	nextBlock()
{
	for (int i = 0; i < BOARD_ROW; i++)
	{
		if (i < BOARD_COL)
		{
			map[0][i] = -1;
			map[BOARD_ROW - 1][i] = -1;
		}
		map[i][0] = -1;
		map[i][BOARD_COL - 1] = -1;
	}

	for (int i = 0; i < NEXTBLOCKBOARD_COL; i++)
	{
		if (i < NEXTBLOCKBOARD_ROW)
		{
			nextBlockBoard[i][0] = -1;
			nextBlockBoard[i][NEXTBLOCKBOARD_COL -1] = -1;
		}
		nextBlockBoard[0][i] = -1;
		nextBlockBoard[NEXTBLOCKBOARD_ROW -1][i] = -1;
	}

}

Tetris::~Tetris()
{
	//destroyBlock();
}

void Tetris::createBlock()
{
	if (existBlock(block))
		return;

	else if (!existBlock(nextBlock))
	{
		createNextBlock();
	}

	block = nextBlock;

	for (int i = 0; i < 4; i++)
	{
		block.position[i].X += BOARD_COL / 2 -2;
		//block.position[i][0] += 1;

		map[block.position[i].Y][block.position[i].X] = block.color;
	}

	createNextBlock();
}

void Tetris::createNextBlock()
{
	if (existBlock(nextBlock))
	{
		for (int i = 0; i < 4; i++)
		{
			nextBlockBoard[nextBlock.position[i].Y][nextBlock.position[i].X] = 0;
		}
	}
	srand(time(NULL));
	int shape = rand() % 7;

	nextBlock = tetrominos[shape];

	
	for (int i = 0; i < 4; i++)
	{
		nextBlock.position[i].Y += 1;
		nextBlock.position[i].X += 1;
		nextBlockBoard[nextBlock.position[i].Y][nextBlock.position[i].X] = nextBlock.color;
	}
}

bool Tetris::moveBlock(MOVE move)
{
	if (checkMoveBlock(move))
		return false;


	for (int i = 0; i < 4; i++)
	{
		map[block.position[i].Y][block.position[i].X] = 0;
	}

	for (int i = 0; i < 4; i++)
	{
		if (move == MOVE::DOWN)
		{
			block.position[i].Y += 1;
		}

		else if (move == MOVE::LEFT)
		{
			block.position[i].X -= 1;
		}

		else if (move == MOVE::RIGHT)
		{
			block.position[i].X += 1;
		}

		map[block.position[i].Y][block.position[i].X] = block.color;
	}

	return true;
}

bool Tetris::rotationBlock()
{
	for (int i = 0; i < 4; i++)
	{
		map[block.position[i].Y][block.position[i].X] = 0;
	}

	pair<int, int> wallKick = { 0,0 };
	if (!checkRotateBlock(wallKick))
	{
		for (int i = 0; i < 4; i++)
		{
			map[block.position[i].Y][block.position[i].X] = block.color;
		}
		return false;
	}
		

	int translateY = block.position[0].Y - block.rotation[0].Y;
	int translateX = block.position[0].X - block.rotation[0].X;

	

	int size = 2;
	if (block.type == BLOCKTYPE::I)
		size = 3;


	for (int i = 0; i < 4; i++)
	{
		int xpos = block.rotation[i].X;
		block.rotation[i].X = size - block.rotation[i].Y;
		block.rotation[i].Y = xpos;

		block.position[i].Y = block.rotation[i].Y + translateY + wallKick.second;
		block.position[i].X = block.rotation[i].X + translateX + wallKick.first;

		map[block.position[i].Y][block.position[i].X] = block.color;
	}

	return true;
}

void Tetris::destroyBlock()
{
	block.type = BLOCKTYPE::NONE;

	for (int i = 0; i < 4; i++)
	{
		map[block.position[i].Y][block.position[i].X]
			= -block.color;
	}
}

bool Tetris::checkMoveBlock(MOVE move)
{
	int xdir = 0, ydir = 0;

	if (move == MOVE::DOWN)
	{
		xdir = 0, ydir = 1;
	}
	else if (move == MOVE::LEFT)
	{
		xdir = -1, ydir = 0;
	}
	else if (move == MOVE::RIGHT)
	{
		xdir = 1, ydir = 0;
	}

	for (int i = 0; i < 4; i++)
	{
		int xpos = block.position[i].X + xdir;
		int ypos = block.position[i].Y + ydir;
		if (map[ypos][xpos] < 0)
		{
			return true;
		}
	}
	return false;
}

bool Tetris::checkRotateBlock(pair<int, int>& wallKick)
{
	if (block.type == BLOCKTYPE::O)
		return false;


	int rotateState = 0;
	vector<Point> matrix = block.rotation;

	Tetromino compareBlock = I_;
	for (int i = 0; i < TETROMINO_NUM; i++)
	{
		if (block.type == tetrominos[i].type)
			compareBlock = tetrominos[i];
	}

	for (int n = 0; n < 4; n++)
	{
		if (matrix == compareBlock.rotation)
		{
			rotateState = n==0 ? 0 : 4 - n;
			break;
		}

		matrix = rotateMatrix(matrix);
	}

	vector<pair<int, int>> wallKickData = 
		tetrisInfo.getSRSwallKickData(block.type, rotateState);

	
	int translateY = block.position[0].Y - block.rotation[0].Y;
	int translateX = block.position[0].X - block.rotation[0].X;

	for (int i = 0; i < wallKickData.size(); i++)
	{
		matrix = rotateMatrix(block.rotation);
		matrix = moveMatrix(wallKickData[i], matrix);
		matrix = moveMatrix(make_pair(translateX, translateY), matrix);

		if (mapIndexBlockCanExist(matrix))
		{
			wallKick = wallKickData[i];
			return true;
		}

	}

	return false;
}

bool Tetris::mapIndexBlockCanExist(vector<Point> index)
{
	
	for (int i = 0; i < index.size(); i++)
	{
		if (index[i].Y < 0 || index[i].X < 0 || index[i].Y >= BOARD_ROW || index[i].X >= BOARD_COL)
			return false;
		else if (map[index[i].Y][index[i].X] != 0)
			return false;
	}
	return true;
}

int Tetris::blockTouchWall(MOVE move)
{
	if (blockTouchBottom())
	{
		if (blockMoveSide(move))
			return TOUCH::BOTTOM | TOUCH::SIDE;
		else
			return TOUCH::BOTTOM;
	}
	else if (blockMoveSide(move))
		return TOUCH::SIDE;

	return TOUCH::NOTOUCH;
}

bool Tetris::blockTouchBottom()
{
	for (int i = 0; i < 4; i++)
	{
		int xpos = block.position[i].X;
		int ypos = block.position[i].Y + 1;
		if (map[ypos][xpos] < 0)
			return true;
	}
	return false;
}

bool Tetris::blockMoveSide(MOVE move)
{
	int xdir = 0, ydir = 0;

	if (move == MOVE::LEFT)
	{
		xdir = -1, ydir = 0;
	}
	else if (move == MOVE::RIGHT)
	{
		xdir = 1, ydir = 0;
	}

	for (int i = 0; i < 4; i++)
	{
		int xpos = block.position[i].X + xdir;
		int ypos = block.position[i].Y + ydir;
		if (map[ypos][xpos] < 0)
		{
			return true;
		}
	}
	return false;
}

void Tetris::drawGhostPiece()
{
	for (int i = 1; i < BOARD_COL; i++)
	{
		for (int j = 1; j < BOARD_ROW; j++)
		{
			int xpos = i;
			int ypos = j;
			if (map[ypos][xpos] == 1)
			{
				map[ypos][xpos] = 0;
			}
		}
	}

	for (int i = 0; i < BOARD_ROW; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			int xpos = block.position[j].X;
			int ypos = block.position[j].Y + i;
			if (map[ypos][xpos] < 0)
			{
				for (int k = 0; k < 4; k++)
				{
					int nxpos = block.position[k].X;
					int nypos = block.position[k].Y + i - 1;

					if (map[nypos][nxpos] != 0)
						return;
					map[nypos][nxpos] = 1;
				}
				return;
			}
		}
	}
}

void Tetris::eraseGhostPiece()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = block.position[i].Y; j < BOARD_ROW; j++)
		{
			int xpos = block.position[i].X;
			int ypos = j;
			if (map[ypos][xpos] == 1)
			{
				map[ypos][xpos] = 0;
			}
		}
	}
}

int Tetris::eraseLine()
{
	int eraseCount = 0;
	for (int i = 1; i < BOARD_ROW - 1; i++)
	{
		if (eraseOneLine(i))
		{
			eraseCount++;
		}
	}

	return eraseCount;
}

bool Tetris::eraseOneLine(int line)
{
	for (int i = 1; i < BOARD_COL - 1; i++)
	{
		if (map[line][i] > -1)
		{
			return false;
		}
	}

	for (int i = line; i > 1; i--)
	{
		for (int j = 1; j < BOARD_COL - 1; j++)
		{
			map[i][j] = map[i - 1][j];
		}
	}

	for (int i = 1; i < BOARD_COL - 1; i++)
	{
		map[1][i] = 0;
	}

	return true;
}

vector<Point> Tetris::rotateMatrix(vector<Point> matrix)
{
	vector<Point> rotationMatrix(matrix.size());

	int size = 2;
	if (block.type == BLOCKTYPE::I)
		size = 3;


	for (int i = 0; i < 4; i++)
	{
		rotationMatrix[i].Y = matrix[i].X;
		rotationMatrix[i].X = size - matrix[i].Y;
	}

	return rotationMatrix;
}

vector<Point> Tetris::moveMatrix(pair<int, int> move, vector<Point> matrix)
{

	for (int i = 0; i < 4; i++)
	{
		matrix[i].X += move.first;
		matrix[i].Y += move.second;
	}

	return matrix;
}