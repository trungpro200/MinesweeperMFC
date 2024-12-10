#include "pch.h"
#include "Score.h"

Score::Score() : SpritesSheet(L"res/score.png")
{
	setSize(23, 13);
	score = 69;
}

void Score::draw(CPaintDC& dc)
{
	int a = max(score,0);
	for (int i = 0; i < 3; i++) {
		getSprite(a % 10, 0)->blit(dc, pos.x + spriteWidth * (2-i), pos.y);
		a /= 10;
	}
}
