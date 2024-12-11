#include "pch.h"
#include "Score.h"

Score::Score() : SpritesSheet(L"res/score.png")
{
	setSize(23, 13);
	score = 0;
}

void Score::draw(CPaintDC& dc)
{
	int a = max(score,0);
	for (int i = 0; i < 3; i++) {
		getSprite(a % 10, 0)->blit(dc, pos.x + spriteWidth * (2-i), pos.y);
		a /= 10;
	}
}

void Score::draw(CClientDC& dc)
{
	int a = max(score, 0);
	for (int i = 0; i < 3; i++) {
		getSprite(a % 10, 0)->blit(dc, pos.x + spriteWidth * (2 - i), pos.y);
		a /= 10;
	}
}

void Score::rightAlign(CPoint origin, CPoint dim)
{
	pos.x = origin.x + dim.x - spriteWidth * 3;
	pos.y = 10;
}

void Score::leftAlign(CPoint origin, CPoint dim)
{
	pos.x = origin.x;
	pos.y = 10;
}
