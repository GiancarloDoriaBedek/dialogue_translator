#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "DialogueTranslationApp.h"
#endif

#include "JSONDoc.h"
#include "MainView.h"
#include "mainfrm.h"
#include "JsonEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// MainView

IMPLEMENT_DYNCREATE(MainView, CView)

BEGIN_MESSAGE_MAP(MainView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// MainView construction/destruction

MainView::MainView() 
{
}

MainView::~MainView()
{
}

BOOL MainView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

// MainView drawing

void MainView::OnDraw(CDC* pDC)
{
}

void MainView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

#ifdef _DEBUG
void MainView::AssertValid() const
{
	CView::AssertValid();
}

void MainView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

JSONDoc* MainView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(JSONDoc)));
	return (JSONDoc*)m_pDocument;
}
#endif //_DEBUG
