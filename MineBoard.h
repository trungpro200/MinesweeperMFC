#pragma once

#include "Score.h"
#include "SpritesSheet.h"
#include <algorithm>
#include <atltypes.h>
#include <vector>
#include <mmsystem.h>

#pragma comment(lib, "Winmm.lib")

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
	int gradient = 0; //Number of bomb surround this tile (0-8)
	bool requireUpdate = true;
	CPoint pos;
};

class GameClass
{
private:
	//Allocate and assign tile's position
	void createBoard();
	//Used to free up memory
	void deleteBoard();
	void drawTile(CPaintDC& dc, int STATE, int x, int y);
public:
	SpritesSheet tilestates;
	
	int size; //Size of the game eg: 16x16
	Tile** tiles;
	
	CPoint pos;
	CPoint sel; //current selected tile

	Score bombLeft;
	Score timePassed;
	
	//Board sizes
	int width; //width of the board in pixel
	int height;//height of the board in pixel
	//Bomb count
	int bomb;
	int tileLeft;

	//Init
	bool finished;//Gamestate
	bool started;//Whether the game started or not
	bool win;//Whether you have skill issues or not

	bool onExcavate;

	GameClass(int size);
	~GameClass();

	void setPos(int x, int y);

	void generateBombs(int count, CPoint bl);
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
	
	//The name tell the usage :/
	Tile& getTile(CPoint pt);

	//When the game end
	void finishGame(bool win); 
	
	//Moore Neighbour
	std::vector<Tile*> getNeighbour(CPoint pos);

	//return how many neighbour which matched given state
	int queryNeighbour(CPoint pos, int state);
	/*int getNeigbourFlags(CPoint pos);
	int getUnknownNeighbour(CPoint pos);*/

	void restartGame();
	void startGame(CPoint pos);

	//Sound
	void sound(LPCTSTR src);
};