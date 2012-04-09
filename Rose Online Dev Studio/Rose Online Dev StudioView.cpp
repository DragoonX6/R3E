// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://msdn.microsoft.com/officeui.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// Rose Online Dev StudioView.cpp : implementation of the CRoseOnlineDevStudioView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Rose Online Dev Studio.h"
#endif

#include "Rose Online Dev StudioDoc.h"
#include "Rose Online Dev StudioView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRoseOnlineDevStudioView

IMPLEMENT_DYNCREATE(CRoseOnlineDevStudioView, CView)

BEGIN_MESSAGE_MAP(CRoseOnlineDevStudioView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CRoseOnlineDevStudioView construction/destruction

CRoseOnlineDevStudioView::CRoseOnlineDevStudioView()
{
	// TODO: add construction code here

}

CRoseOnlineDevStudioView::~CRoseOnlineDevStudioView()
{
}

BOOL CRoseOnlineDevStudioView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CRoseOnlineDevStudioView drawing

void CRoseOnlineDevStudioView::OnDraw(CDC* /*pDC*/)
{
	CRoseOnlineDevStudioDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CRoseOnlineDevStudioView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CRoseOnlineDevStudioView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CRoseOnlineDevStudioView diagnostics

#ifdef _DEBUG
void CRoseOnlineDevStudioView::AssertValid() const
{
	CView::AssertValid();
}

void CRoseOnlineDevStudioView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRoseOnlineDevStudioDoc* CRoseOnlineDevStudioView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRoseOnlineDevStudioDoc)));
	return (CRoseOnlineDevStudioDoc*)m_pDocument;
}
#endif //_DEBUG


// CRoseOnlineDevStudioView message handlers
