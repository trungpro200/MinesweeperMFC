#include "pch.h"
#include "Sprite.h"

Sprite::Sprite(LPCTSTR src) 
{
	/*image.Load(src);*/
	CImage::Load(src);
}

Sprite::Sprite(CImage image)
{
	/*this->image = image;*/
	//CImage(image);
}

Sprite::Sprite()
{
}

void Sprite::blit(CPaintDC& dc, int x, int y)
{
	static int a = 0;

	a++;
	CDC* img = CDC::FromHandle(CImage::GetDC());

	dc.BitBlt(x, y, CImage::GetWidth(), CImage::GetHeight(), img, 0, 0, SRCCOPY);

	CImage::ReleaseDC();

	//TRACE("Tile updated: %i\n", a);
}
