#pragma once

#include "JsonEdit.h"
#include <map>

class CMainFrame : public CFrameWndEx
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	CMFCRibbonBar     m_wndRibbonBar;
protected:  // control bar embedded members
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	CMFCRibbonStatusBar  m_wndStatusBar;
	JsonEditControl m_leftJsonEditControl;
	JsonEditControl m_rightJsonEditControl;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnOpenFirstResourceFile();
	afx_msg void OnOpenSecondResourceFile();
	std::map<CString, CString> ResourceA;
	std::map<CString, CString> ResourceB;

private:
	std::map<CString, CString> LoadResourceJSONFile(CString jsonContent);
};


