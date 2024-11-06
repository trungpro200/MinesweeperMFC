#include "pch.h"
#include "SpritesSheet.h"

SpritesSheet::SpritesSheet() : Sprite(_T("res/2000.png"))
{
	spriteHeight = 1;
	spriteWidth = 1;
	sprites = nullptr;
}

SpritesSheet::SpritesSheet(LPCTSTR src) : Sprite(src)
{
	spriteHeight = 1;
	spriteWidth = 1;
	sprites = nullptr;
}

void SpritesSheet::setSize(int h, int w) //Set the size of the sprite and load them
{
	spriteHeight = h;
	spriteWidth = w;

	int row = image.GetHeight() / h;
	int col = image.GetWidth() / w;

	sprites = new Sprite * [row];

	for (int i = 0; i < col; i++) {
		sprites[i] = new Sprite[col];
	}

	CDC* src = CDC::FromHandle(image.GetDC());

	for (int i = 0; i < col; i++)
		for (int j = 0; j < row; j++) {
			sprites[i][j].image.Create(w, h, image.GetBPP());

			CDC* dest = CDC::FromHandle(sprites[i][j].image.GetDC());

			dest->BitBlt(0, 0, w, h, src, j * 16, i * 16, SRCCOPY);

			sprites[i][j].image.ReleaseDC();
		}
}

Sprite* SpritesSheet::getSprite(int x, int y)
{
	if (sprites == nullptr) {
		return nullptr;
	}

	return &sprites[y][x];
}
