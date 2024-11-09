#include "pch.h"
#include "Face.h"

Face::Face() : SpritesSheet(_T("res/faces.png"))
{
	setSize(36, 36);

	currentState = NORMAL;
	requireUpdate = true;
}

void Face::setState(FACE state)
{
	if (state == currentState)
		return;

	currentState = state;
	requireUpdate = true;
}

void Face::setPos(MineBoard* mb)
{
	pos.x = (mb->width / 2) + mb->pos.x - (spriteWidth / 2);
	this->mb = mb;
}

void Face::draw(CPaintDC& dc)
{
	if (!requireUpdate)
		return;
	Sprite* a = getSprite(currentState, 0);
	a->blit(dc, pos.x, pos.y);
}

bool Face::containPoint(CPoint pt)
{
	if (pt.x < pos.x || pt.y < pt.y)
		return false;

	if (pt.x > (pos.x + spriteWidth) || pt.y > (pos.y + spriteHeight))
		return false;

	return true;
}

void Face::clickDown(CPoint pt)
{
	if (containPoint(pt)) {
		setState(SELECTED);
	}
}

void Face::clickUp(CPoint pt)
{
	setState(NORMAL);
}

void Face::mouseMove(CPoint pt)
{
	if (currentState != SELECTED && currentState != NORMAL)
		return;

	if (containPoint(pt)) {
		setState(SELECTED);
	}
	else {
		setState(NORMAL);
	}
}
