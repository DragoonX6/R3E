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

// Rose Online Dev StudioView.h : interface of the CRoseOnlineDevStudioView class
//

#pragma once


class CRoseOnlineDevStudioView : public CView
{
protected: // create from serialization only
	CRoseOnlineDevStudioView();
	DECLARE_DYNCREATE(CRoseOnlineDevStudioView)

// Attributes
public:
	CRoseOnlineDevStudioDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CRoseOnlineDevStudioView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in Rose Online Dev StudioView.cpp
inline CRoseOnlineDevStudioDoc* CRoseOnlineDevStudioView::GetDocument() const
   { return reinterpret_cast<CRoseOnlineDevStudioDoc*>(m_pDocument); }
#endif

