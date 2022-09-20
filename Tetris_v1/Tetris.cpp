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

	for (int i = 0; i < TILE_NUM; i++)
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
		for (int i = 0; i < TILE_NUM; i++)
		{
			nextBlockBoard[nextBlock.position[i].Y][nextBlock.position[i].X] = 0;
		}
	}

	//generator
	srand(time(NULL));
	int shape = rand() % 7;

	nextBlock = tetrominos[shape];

	
	for (int i = 0; i < TILE_NUM; i++)
	{
		nextBlock.position[i].Y += 1;
		nextBlock.position[i].X += 1;
		nextBlockBoard[nextBlock.position[i].Y][nextBlock.position[i].X] = nextBlock.color;
	}
}

void Tetris::destroyBlock()
{
	if (!existBlock(block))
		return;

	for (int i = 0; i < TILE_NUM; i++)
	{
		map[block.position[i].Y][block.position[i].X]
			= -block.color;
	}

	block = Tetromino();
}

bool Tetris::moveBlock(const Point& p)
{
	vector<Point> _position = block.position;

	for (int i = 0; i < TILE_NUM; i++)
	{
		_position[i] += p;
	}

	if (!canBlockPutThisPoints(_position))
		return false;

	for (int i = 0; i < TILE_NUM; i++)
	{
		map[block.position[i].Y][block.position[i].X] = 0;
	}
	for (int i = 0; i < TILE_NUM; i++)
	{
		map[_position[i].Y][_position[i].X] = block.color;
	}

	block.position = _position;

	return true;
}

bool Tetris::rotateBlockRight()
{
	Tetromino _block = canRotateBlock();
	if (_block.type == BLOCKTYPE::NONE)
	{
		return false;
	}
		
	Point translate = getTranslateDistance();

	for (int i = 0; i < TILE_NUM; i++)
	{
		map[block.position[i].Y][block.position[i].X] = 0;
	}

	for (int i = 0; i < TILE_NUM; i++)
	{
		map[_block.position[i].Y][_block.position[i].X] = block.color;
	}

	block = _block;

	return true;
}

Tetromino Tetris::canRotateBlock()
{
	if (block.type == BLOCKTYPE::O)
		return Tetromino();

	int rotateState = 0;
	vector<Point> rotationPoints = block.rotation;
	Tetromino rotationBlock = block;
	Tetromino compareBlock = tetrominos[block.type];
	
	for (int n = 0; n < 4; n++)
	{
		if (rotationBlock.rotation == compareBlock.rotation)
		{
			rotateState = n;
			break;
		}

		compareBlock.rotation = rotatePoints(compareBlock.rotation);
	}

	vector<pair<int, int>> wallKickData = 
		tetrisInfo.getSRSwallKickData(block.type, rotateState);

	Point translateDist = getTranslateDistance();

	for (int i = 0; i < wallKickData.size(); i++)
	{
		Point p = { wallKickData[i].second + translateDist.Y,
			wallKickData[i].first + translateDist.X };

		rotationBlock.rotation = rotatePoints(block.rotation);
		rotationBlock.position = movePoints(p, rotationBlock.rotation);

		if (canBlockPutThisPoints(rotationBlock.position))
		{
			return rotationBlock;
		}

	}

	return Tetromino();
}

bool Tetris::canBlockPutThisPoints(vector<Point> points)
{
	for (int i = 0; i < points.size(); i++)
	{
		if (points[i].Y < 0 || points[i].X < 0 ||
			points[i].Y >= BOARD_ROW || points[i].X >= BOARD_COL)
			return false;
		else if (map[points[i].Y][points[i].X] < 0)
			return false;
	}
	return true;
}

bool Tetris::blockTouchBottom()
{
	if (block == Tetromino())
		return false;

	for (int i = 0; i < TILE_NUM; i++)
	{
		int xpos = block.position[i].X;
		int ypos = block.position[i].Y + 1;
		if (map[ypos][xpos] < 0)
			return true;
	}
	return false;
}

void Tetris::drawGhostPiece()
{
	if (!existBlock(block))
		return;

	for (int i = 0; i < TILE_NUM; i++)
	{
		if(map[block.ghostpiecePosition[i].Y][block.ghostpiecePosition[i].X] == 1)
			map[block.ghostpiecePosition[i].Y][block.ghostpiecePosition[i].X] = 0;
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
					block.ghostpiecePosition[k].X = block.position[k].X;
					block.ghostpiecePosition[k].Y = block.position[k].Y + i - 1;

					if (map[block.ghostpiecePosition[k].Y][block.ghostpiecePosition[k].X] > 0)
						continue;

					map[block.ghostpiecePosition[k].Y][block.ghostpiecePosition[k].X] = 1;
				}
				return;
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
		if (map[line][i] ==0 || map[line][i] == 1)
		{
			return false;
		}
	}

	destroyBlock();

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

vector<Point> Tetris::rotatePoints(const vector<Point>& points)
{
	vector<Point> rotationPoints(points.size());

	for (int i = 0; i < 4; i++)
	{
		int size = block.type == BLOCKTYPE::I ? 3 : 2;
		rotationPoints[i] = { points[i].X, size - points[i].Y };
	}

	return rotationPoints;
}

vector<Point> Tetris::movePoints(const Point& move, const vector<Point>& points)
{
	vector<Point> rotationPoints(points.size());

	for (int i = 0; i < 4; i++)
	{
		rotationPoints[i] = points[i];
		rotationPoints[i] += move;
	}

	return rotationPoints;
}