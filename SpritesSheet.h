#pragma once
#include "Sprite.h"

class SpritesSheet : public Sprite
{
private:
	int spriteHeight, spriteWidth;
	Sprite** sprites;
public:
	SpritesSheet();
	SpritesSheet(LPCTSTR src);

	void setSize(int h, int w);
	Sprite* getSprite(int x, int y);
};

