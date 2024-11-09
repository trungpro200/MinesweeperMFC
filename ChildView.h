
// ChildView.h : interface of the CChildView class
//
#pragma once
#include "MineBoard.h"
#include "Face.h"


// CChildView window

class CChildView : public CWnd
{
// Construction
public:
	CChildView();

// Attributes
public:
	MineBoard game;
	Face face;
	bool holdingL;		//Whether LButton is down or not
	bool background;	//Whether drew background or not
// Operations
public:

// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
//	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};

