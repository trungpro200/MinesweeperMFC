
// ChildView.cpp : implementation of the CChildView class
//

#include "pch.h"
#include "framework.h"
#include "MinesweeperMFC.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildView

CChildView::CChildView() : game(16)
{
	game.setPos(10, 36);
	face.setPos(&game);

	holdingL = false;
	background = false;
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.style &= ~WS_THICKFRAME;

	cs.cx = game.width + game.pos.x * 2;
	cs.cy = game.height + game.pos.y * 2;

	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		::LoadCursor(nullptr, IDC_ARROW),
		reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), nullptr);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	if (!background) {
		background = true;
		CRect tp;

		GetClientRect(&tp);

		dc.FillRect(tp, &CBrush(RGB(190, 190, 190)));
	}
	
	game.draw(dc);
	//face.getSprite(NORMAL, 0)->blit(dc, 0, 0);
	face.draw(dc);
}



void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//if (game.finished)
	//	return;

	game.clickDown(point);

	if (game.screenToBoard(point).x != -1) {
		face.setState(OPENING);
	}
	face.clickDown(point);

	holdingL = true;

	Invalidate(0);
	CWnd::OnLButtonDown(nFlags, point);
}


void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	/*if (game.finished)
		return;*/
	holdingL = false;

	face.clickUp(point);
	game.clickUp(point);

	if (game.finished) {
		if (game.win) {
			face.setState(WON);
		}
		else {
			face.setState(DEAD);
		}
	}

	Invalidate(0);
	CWnd::OnLButtonUp(nFlags, point);
}


void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (!holdingL) {
		return;
	}
	face.mouseMove(point);
	game.mouseMove(point);
	Invalidate(0);
	CWnd::OnMouseMove(nFlags, point);
}


//void CChildView::OnRButtonDown(UINT nFlags, CPoint point)
//{
	// TODO: Add your message handler code here and/or call default
	
//	CWnd::OnRButtonDown(nFlags, point);
//}


void CChildView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (holdingL) {
		return;
	}

	game.rightClick(point);
	Invalidate(0);

	CWnd::OnRButtonUp(nFlags, point);
}
