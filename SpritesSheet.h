#pragma once
#include "Sprite.h"

class SpritesSheet : public Sprite
{
private:
	int spriteHeight, spriteWidth;
	int row, col;
	Sprite** sprites;
public:
	SpritesSheet();
	SpritesSheet(LPCTSTR src);
	~SpritesSheet();

	void createSprites(int row, int col);

	void setSize(int h, int w);
	Sprite* getSprite(int x, int y);
};

