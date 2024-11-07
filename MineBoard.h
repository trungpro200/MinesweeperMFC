#pragma once
#include "SpritesSheet.h"
#include <algorithm>
#include <atltypes.h>

#define NOBOMB_TILE 0
#define UNKNOWN_TILE -1
#define BOMB_TILE -2
#define EXPLODED_TILE -3
#define SELECTED_TILE -4

//The remain is for surrounded by n-bomb cases//

class MineBoard
{
private:
	void createBoard();
	void deleteBoard();
	void drawTile(CPaintDC& dc, int STATE, int x, int y);
public:
	SpritesSheet spritesSheet;
	
	int size; //Size of the game eg: 16x16
	bool** board; //Store which tile has bomb
	int** tilesState; 
	
	CPoint pos;
	CPoint sel; //current selected tile

	MineBoard(int size);
	~MineBoard();

	void setPos(int x, int y);

	//rate 0-1, 0 mean no bomb, 1 mean all tiles have bomb :skull:
	void generateBombs(double rate); 

	void draw(CPaintDC& dc);

	//Mouse util
	void clickDown(CPoint point);
	void clickUp(CPoint point);
	void mouseMove(CPoint point);

	//return pos of clicked grid of the board, return (-1,-1) if out of bound
	CPoint screenToBoard(CPoint screenPos); 

	//return state of a tile at pos x, y
	int getState(CPoint pt) const;
	void setState(CPoint pt, int state);
};

