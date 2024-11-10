#pragma once
#include "SpritesSheet.h"
#include <algorithm>
#include <atltypes.h>
#include <vector>

#define NOBOMB_TILE 0
#define UNKNOWN_TILE -1
#define BOMB_TILE -2
#define EXPLODED_TILE -3
#define SELECTED_TILE -4
#define ERRORTYPE -5
#define FLAGGED -6
#define WRONG_FLAG -7

//The remain is for surrounded by n-bomb cases//
struct Tile {
	bool haveBomb = 0;
	int state = UNKNOWN_TILE;
	int gradient = 8; //Number of bomb surround this tile (0-8)
	bool requireUpdate = true;
	CPoint pos;
};

class MineBoard
{
private:
	void createBoard();
	void deleteBoard();
	void drawTile(CPaintDC& dc, int STATE, int x, int y);
public:
	SpritesSheet spriteSheet;
	
	int size; //Size of the game eg: 16x16
	Tile** tiles;
	
	CPoint pos;
	CPoint sel; //current selected tile

	//Board sizes
	int width; //width of the board in pixel
	int height;//height of the board in pixel
	//Bomb count
	int bomb;

	//Init
	bool finished;//Gamestate
	bool started;
	bool win;

	MineBoard(int size);
	~MineBoard();

	void setPos(int x, int y);

	//rate 0-1, 0 mean no bomb, 1 mean all tiles have bomb :skull:
	void generateBombs(double rate); 
	void generateBombs(int count, CPoint bl);

	//Calculate the number of bomb surround a tile at pos
	//Also assign the pos of the tile
	void calcGradient(CPoint pos);

	void draw(CPaintDC& dc);

	//Mouse util
	void clickDown(CPoint point);
	void clickUp(CPoint point);
	void mouseMove(CPoint point);
	void rightClick(CPoint point);

	//Flagging
	void flagTile(CPoint pos);

	//Open up tile at x, y pos
	void openTile(CPoint pos);

	//return pos of clicked grid of the board, return (-1,-1) if out of bound
	CPoint screenToBoard(CPoint screenPos); 

	//return state of a tile at pos x, y
	int getState(CPoint pt) const;

	void setState(Tile& tile, int state);
	void setState(CPoint pt, int state);
	
	//The name tell the behavior :/
	Tile& getTile(CPoint pt);

	//When the game end
	void finishGame(bool win); 
	
	//Moore Neighbour
	std::vector<Tile*> getNeighbour(CPoint pos);

	void restartGame();
	void startGame(CPoint pos);
};

