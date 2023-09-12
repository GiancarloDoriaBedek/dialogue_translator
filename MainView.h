#pragma once
#include "JsonEdit.h";

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
	virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:

// Implementation
public:
	virtual ~MainView();
	CString ResourceAPath;
	std::map<CString, CString> ResourceA;
	std::map<CString, CString> ResourceB;
protected:
	JsonEditControl* m_leftJsonEditControl;
	JsonEditControl* m_rightJsonEditControl;
	virtual void OnInitialUpdate();
	afx_msg void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnEnSetfocusJsonEditControl();
	afx_msg void OnOpenFirstResourceFile();
	afx_msg void OnOpenSecondResourceFile();
	std::map<CString, CString> LoadResourceJSONFile(CString jsonContent);

	LRESULT OnUpdateLeftJson(WPARAM wParam, LPARAM lParam);
	LRESULT OnUpdateRightJson(WPARAM wParam, LPARAM lParam);
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

