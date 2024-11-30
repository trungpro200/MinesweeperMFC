#pragma once
#include "Sprite.h"

class SpritesSheet : public Sprite
{
private:
	int row, col;
	Sprite** sprites;
public:
	int spriteHeight, spriteWidth;
	SpritesSheet();
	SpritesSheet(LPCTSTR src);
	~SpritesSheet();

	void allocateSprites(int row, int col);

	void setSize(int h, int w);
	Sprite* getSprite(int x, int y);
};

