#pragma once
class Sprite : public CImage
{
public:
	//CImage image;

	Sprite(LPCTSTR src);
	Sprite(CImage image);
	Sprite();

	void blit(CPaintDC& dc, int x, int y);
	void blit(CClientDC& dc, int x, int y);
};

