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
	createNextBlock();

	for (int i = 0; i < TILE_NUM; i++)
	{
		block.position[i].X += BOARD_COL / 2 - 1;
		block.position[i].Y += 1;
	}

	putBlockOnMap();
}

void Tetris::createNextBlock()
{
	if (existBlock(nextBlock))
	{
		for (int i = 0; i < TILE_NUM; i++)
		{
			int Y = nextBlock.position[i].Y + 1;
			int X = nextBlock.position[i].X + 1;

			nextBlockBoard[Y][X] = 0;
		}
	}

	nextBlock = getRandomBlock();

	
	for (int i = 0; i < TILE_NUM; i++)
	{
		int Y = nextBlock.position[i].Y + 1;
		int X = nextBlock.position[i].X + 1;

		nextBlockBoard[Y][X] = nextBlock.color;
	}
}

void Tetris::lockBlock()
{
	if (!existBlock(block))
		return;

	for (int i = 0; i < TILE_NUM; i++)
	{
		int Y = block.position[i].Y;
		int X = block.position[i].X;

		map[Y][X] = -block.color;
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

	eraseBlockOnMap();
	block.position = _position;
	putBlockOnMap();
	
	return true;
}

bool Tetris::rotateBlockRight()
{
	Tetromino _block = canRotateBlock();
	if (_block.type == BLOCKTYPE::NONE)
	{
		return false;
	}

	eraseBlockOnMap();
	block = _block;
	putBlockOnMap();

	return true;
}

int Tetris::blockTouchBottom()
{
	if (block == Tetromino())
		return eraseLine();

	for (int i = 0; i < TILE_NUM; i++)
	{
		int Y = block.position[i].Y + 1;
		int X = block.position[i].X;

		if (map[Y][X] < 0)
		{
			return eraseLine();
		}

	}

	return -1;
}

void Tetris::boardReset()
{
	for (int i = 0; i < BOARD_ROW; i++)
	{
		for (int j = 0; j < BOARD_COL; j++)
		{
			if (map[i][j] != -1)
				map[i][j] = 0;
		}
	}
}

Tetromino Tetris::getRandomBlock()
{
	if (_7Bag.empty())
	{
		_7Bag = tetrominos;
		shuffle(_7Bag.begin(),
			_7Bag.end(),
			random_device());
	}

	Tetromino tet = _7Bag.back();
	_7Bag.pop_back();

	return tet;
}

Tetromino Tetris::canRotateBlock()
{
	if (block.type == BLOCKTYPE::O||
		block.type == BLOCKTYPE::NONE)
		return Tetromino();

	int rotateState = 0;
	vector<Point> rotationPoints = block.rotation;
	Tetromino rotationBlock = block;
	Tetromino compareBlock = tetrominos[block.type];
	
	for (int n = 0; n < 4; n++)
	{
		if (rotationBlock.rotation 
			== compareBlock.rotation)
		{
			rotateState = n;
			break;
		}

		compareBlock.rotation 
			= rotatePoints(compareBlock.rotation);
	}

	vector<pair<int, int>> wallKickData = 
		tetrisInfo.getSRSwallKickData(block.type, rotateState);

	Point translateDist = getTranslateDistance();

	for (int i = 0; i < wallKickData.size(); i++)
	{
		Point p = { 
			wallKickData[i].second + translateDist.Y,
			wallKickData[i].first + translateDist.X 
		};

		rotationBlock.rotation 
			= rotatePoints(block.rotation);
		rotationBlock.position 
			= movePoints(p, rotationBlock.rotation);

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
		int Y = points[i].Y;
		int X = points[i].X;

		if (Y < 0 || X < 0 ||
			Y >= BOARD_ROW || 
			X >= BOARD_COL)
		{
			return false;
		}
		else if (map[Y][X] < 0)
		{
			return false;
		}
			
	}
	return true;
}



bool Tetris::hardDrop()
{
	eraseBlockOnMap();
	block.position = block.ghostPiecePos;
	lockBlock();

	return true;
}

void Tetris::putBlockOnMap()
{
	coloringMap(block.color);

	drawGhostPiece();
}

void Tetris::eraseBlockOnMap()
{
	coloringMap(0);
}

void Tetris::coloringMap(int color)
{
	int Y = 0, X = 0;

	for (int i = 0; i < TILE_NUM; i++)
	{
		Y = block.position[i].Y;
		X = block.position[i].X;

		map[Y][X] = color;
	}
}

bool Tetris::toppedOut()
{
	int Y = 0, X = 0;

	for (int i = 0; i < TILE_NUM; i++)
	{
		Y = block.position[i].Y + 1;
		X = block.position[i].X + BOARD_COL / 2 - 1;

		if (map[Y][X] < 0)
			return true;
	}

	return false;
}



void Tetris::drawGhostPiece()
{
	if (!existBlock(block))
		return;

	int gstY = 0, gstX = 0;
	int Y = 0, X = 0;

	for (int i = 0; i < TILE_NUM; i++)
	{
		gstY = block.ghostPiecePos[i].Y;
		gstX = block.ghostPiecePos[i].X;

		if(map[gstY][gstX] == 1)
			map[gstY][gstX] = 0;
	}

	for (int i = 0; i < BOARD_ROW; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			Y = block.position[j].Y + i;
			X = block.position[j].X;
			
			if (map[Y][X] < 0)
			{
				for (int k = 0; k < 4; k++)
				{
					Y = block.position[k].Y + i;
					X = block.position[k].X;

					block.ghostPiecePos[k].Y = Y - 1;
					block.ghostPiecePos[k].X = X;

					gstY = block.ghostPiecePos[k].Y;
					gstX = block.ghostPiecePos[k].X;

					if (map[gstY][gstX] > 0)
						continue;

					map[gstY][gstX] = 1;
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
		if (map[line][i] ==0 ||
			map[line][i] == 1)
		{
			return false;
		}
	}

	lockBlock();

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