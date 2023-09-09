#pragma once

class MainView : public CView
{
protected: // create from serialization only
	MainView();
	DECLARE_DYNCREATE(MainView)

// Attributes
public:
	JSONDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~MainView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:

};

#ifndef _DEBUG  // debug version in DialogueTranslatorView.cpp
inline JSONDoc* MainView::GetDocument() const
   { return reinterpret_cast<JSONDoc*>(m_pDocument); }
#endif

