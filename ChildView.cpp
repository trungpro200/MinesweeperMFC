
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
	game.spritesSheet.setSize(24, 24);
	game.setPos(300, 30);
	holdingL = false;
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.style &= ~WS_THICKFRAME;

	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		::LoadCursor(nullptr, IDC_ARROW),
		reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), nullptr);

	

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	game.draw(dc);
}



void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	game.clickDown(point);
	holdingL = true;

	Invalidate(0);
	CWnd::OnLButtonDown(nFlags, point);
}


void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	holdingL = false;

	game.clickUp(point);
	Invalidate(0);
	CWnd::OnLButtonUp(nFlags, point);
}


void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (!holdingL) {
		return;
	}
	game.mouseMove(point);
	Invalidate(0);
	CWnd::OnMouseMove(nFlags, point);
}
