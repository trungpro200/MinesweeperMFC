#pragma once
#include "SpritesSheet.h"
#include <algorithm>

#define NOBOMB_TILE 0
#define UNKNOWN_TILE -1
#define BOMB_TILE -2
#define EXPLODED_TILE -3
//The remain is for surrounded by n-bomb cases//

class MineBoard
{
private:
	void createBoard();
	void deleteBoard();
	void drawTile(CPaintDC& dc, int STATE, int x, int y);
public:
	SpritesSheet spritesSheet;
	
	int size;
	int** board;
	int** tilesState;
	
	CPoint pos;

	MineBoard(int size);
	~MineBoard();

	void setPos(int x, int y);
	void generateBombs(double rate); //rate 0-1, 0 mean no bomb, 1 mean all tiles have bomb :skull:

	void draw(CPaintDC& dc);
};

