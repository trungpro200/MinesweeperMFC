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
	CPoint origin(x * spritesSheet.spriteWidth, y * spritesSheet.spriteHeight);

	switch (STATE) {
	case UNKNOWN_TILE:
		tile = spritesSheet.getSprite(0, 0);
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
			TRACE("%i\n", tilesState[i][j]);
		}
		TRACE("\n");
	}
}
