#include "pch.h"
#include "MineBoard.h"

void MineBoard::createBoard()
{
	if (board != nullptr)
		deleteBoard();

	board = new bool* [size];
	tilesState = new int* [size];
	for (int i = 0; i < size; i++) {
		board[i] = new bool[size];
		tilesState[i] = new int[size];

		std::fill(tilesState[i], tilesState[i] + size, UNKNOWN_TILE);
	}

	//Randomize
	generateBombs(0.2f);
}

void MineBoard::deleteBoard()
{
	if (board == nullptr)
		return;

	for (int i = 0; i < size; i++) {
		delete[] board[i];
		delete[] tilesState[i];
	}
	delete[] board;
	delete[] tilesState;
}

void MineBoard::drawTile(CPaintDC& dc, int STATE, int x, int y)
{
	Sprite* tile;
	CPoint origin(x * spritesSheet.spriteWidth + pos.x, y * spritesSheet.spriteHeight + pos.y);

	switch (STATE) {
	case UNKNOWN_TILE:
		tile = spritesSheet.getSprite(0, 0);
		break;
	case SELECTED_TILE:
		tile = spritesSheet.getSprite(1, 0);
		break;
	case NOBOMB_TILE:
		tile = spritesSheet.getSprite(1, 0);
		break;
	case EXPLODED_TILE:
		tile = spritesSheet.getSprite(6, 0);
		break;
	case BOMB_TILE:
		tile = spritesSheet.getSprite(5, 0);
		break;
	default:
		tile = spritesSheet.getSprite(STATE-1, 1);
		break;
	}

	//TRACE("%i, %i\n", origin.x, origin.y);

	tile->blit(dc, origin.x, origin.y);
}

MineBoard::MineBoard(int size)
{
	this->size = size;
	board = nullptr;
	tilesState = nullptr;
	createBoard();
}

MineBoard::~MineBoard()
{
	deleteBoard();
}

void MineBoard::setPos(int x, int y)
{
	pos.x = x;
	pos.y = y;
}

void MineBoard::generateBombs(double rate)
{
	srand(time(NULL)); //Seeding
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			double f = (double) rand() / RAND_MAX;
			board[i][j] = f < rate;
		}
	}
}

void MineBoard::draw(CPaintDC& dc)
{
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			drawTile(dc, tilesState[i][j], j, i);
			//TRACE("%i\t", board[i][j]);
		}
		//TRACE("\n");
	}
}

//Select only
void MineBoard::clickDown(CPoint point)
{
	CPoint clk = screenToBoard(point);

	if (clk.x == -1)
		return;
	if (getState(clk) != UNKNOWN_TILE)
		return;

	sel = clk;
	
	setState(sel, SELECTED_TILE);
}

//Comfirm sweeping
void MineBoard::clickUp(CPoint point)
{
	sel = CPoint(-1, -1);
}

void MineBoard::mouseMove(CPoint point)
{
	CPoint nPos = screenToBoard(point);

	if (nPos == sel)
		return;

	if (nPos.x == -1) {
		setState(sel, UNKNOWN_TILE);
		return;
	}

	setState(sel, UNKNOWN_TILE);
	setState(nPos, SELECTED_TILE);
	sel = nPos;
}

CPoint MineBoard::screenToBoard(CPoint screenPos)
{
	if (screenPos.x < pos.x || screenPos.y < pos.y) //Upper left boundary
		return CPoint(-1, -1);

	CPoint ret;

	ret.x = (screenPos.x-pos.x) / spritesSheet.spriteWidth;
	ret.y = (screenPos.y-pos.y) / spritesSheet.spriteHeight;

	if (ret.x >= size || ret.y >=size) { //Lower right boundary
		return CPoint(-1, -1);
	}

	TRACE("%i, %i\n", ret.x, ret.y);

	return ret;
}

int MineBoard::getState(CPoint pt) const
{
	return tilesState[pt.y][pt.x];
}

void MineBoard::setState(CPoint pt, int state)
{
	if (pt.x == -1)
		return;
	tilesState[pt.y][pt.x] = state;
}
