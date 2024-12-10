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
};

