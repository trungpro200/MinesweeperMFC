#include "pch.h"
#include "SpritesSheet.h"

SpritesSheet::SpritesSheet() : Sprite(_T("res/2000.png"))
{
	spriteHeight = 1;
	spriteWidth = 1;
	sprites = nullptr;
	row = 1;
	col = 1;
}

SpritesSheet::SpritesSheet(LPCTSTR src) : Sprite(src)
{
	spriteHeight = 1;
	spriteWidth = 1;
	sprites = nullptr;
	row = 1;
	col = 1;
}

SpritesSheet::~SpritesSheet()
{
	int row = GetHeight() / spriteHeight;

	if (sprites == nullptr)
		return;
	for (int i = 0; i < row; i++) {
		delete[] sprites[i];
	}
	delete[] sprites;
}

void SpritesSheet::allocateSprites(int row, int col)
{
	sprites = new Sprite * [row];

	for (int i = 0; i < row; i++) {
		sprites[i] = new Sprite[col];
	}
}
//Set the size of the sprite and load them
void SpritesSheet::setSize(int h, int w) 
{
	spriteHeight = h;
	spriteWidth = w;

	int row = GetHeight() / h;
	int col = GetWidth() / w;

	this->row = row;
	this->col = col;

	allocateSprites(row, col);

	CDC* src = CDC::FromHandle(GetDC());

	for (int i = 0; i < row; i++)
		for (int j = 0; j < col; j++) {
			sprites[i][j].Create(w, h, GetBPP());

			CDC* dest = CDC::FromHandle(sprites[i][j].GetDC());

			dest->BitBlt(0, 0, w, h, src, j * w, i * w, SRCCOPY);

			sprites[i][j].ReleaseDC();
		}
	ReleaseDC();
}

Sprite* SpritesSheet::getSprite(int x, int y)
{
	if (sprites == nullptr) {
		return nullptr;
	}

	return &sprites[y][x];
}
