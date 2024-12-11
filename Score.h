#pragma once
#include "SpritesSheet.h"

class Score :
    public SpritesSheet
{
public:
    Score();

    CPoint pos;

    int score;

    void draw(CPaintDC& dc);
    void draw(CClientDC& dc);

    void rightAlign(CPoint origin, CPoint dim);
    void leftAlign(CPoint origin, CPoint dim);
};

