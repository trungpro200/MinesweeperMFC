#pragma once
#include "SpritesSheet.h"
#include "MineBoard.h"

enum FACE
{
    NORMAL,
    SELECTED,
    OPENING,
    WON, 
    DEAD
};

class Face :
    public SpritesSheet
{
public:
    Face();


    CPoint pos;

    FACE currentState;
    bool requireUpdate;

    MineBoard* mb;

    void setState(FACE state);

    //Align the face with the center of the board
    void setPos(MineBoard* mb);
    //Draw the face according to Face.pos
    void draw(CPaintDC& dc);

    bool containPoint(CPoint pt);

    //Mouse utils
    void clickDown(CPoint pt);
    void clickUp(CPoint pt);
    void mouseMove(CPoint pt);
};

