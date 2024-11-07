#include "pch.h"
#include "MineBoard.h"

void MineBoard::createBoard()
{
	if (board != nullptr)
		deleteBoard();

	board = new int* [size];
	tilesState = new int* [size];
	for (int i = 0; i < size; i++) {
		board[i] = new int[size];
		tilesState[i] = new int[size];

		std::fill(tilesState[i], tilesState[i] + size, UNKNOWN_TILE);
	}

	//Randomize
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
	default:
		return;
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

void MineBoard::draw(CPaintDC& dc)
{
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			drawTile(dc, tilesState[i][j], j, i);
			//TRACE("%i\n", tilesState[i][j]);
		}
		//TRACE("\n");
	}
}

void MineBoard::clickDown(CPoint point)
{
	CPoint clk = screenToBoard(point);

	if (clk.x == -1)
		return;

	sel = clk;
	
	setState(sel, SELECTED_TILE);
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
	tilesState[pt.y][pt.x] = state;
}
