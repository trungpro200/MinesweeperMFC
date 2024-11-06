#include "pch.h"
#include "Sprite.h"

Sprite::Sprite(LPCTSTR src)
{
	image.Load(src);
}

Sprite::Sprite(CImage image)
{
	this->image = image;
}

Sprite::Sprite()
{
}

void Sprite::blit(CPaintDC& dc, int x, int y)
{
	CDC* img = CDC::FromHandle(image.GetDC());

	dc.BitBlt(x, y, image.GetWidth(), image.GetHeight(), img, 0, 0, SRCCOPY);

	image.ReleaseDC();
}
