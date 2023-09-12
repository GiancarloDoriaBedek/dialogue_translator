#include "stdafx.h"
#include "DialogueTranslationApp.h"

#include "MainFrm.h"
#include "JsonEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <map>
#include <string>
#include "JSONDoc.h"

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	//ON_COMMAND(ID_BUTTON_FIRST, &CMainFrame::OnOpenFirstResourceFile)
	//ON_COMMAND(ID_BUTTON_SECOND, &CMainFrame::OnOpenSecondResourceFile)
	//ON_EN_SETFOCUS(IDC_EDIT_JSON, &CMainFrame::OnEnSetfocusJsonEditControl)
END_MESSAGE_MAP()

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLUE);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;
	// set the visual manager and style based on persisted value
	OnApplicationLook(theApp.m_nAppLook);

	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	CString strTitlePane1;
	CString strTitlePane2;
	bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
	ASSERT(bNameValid);
	bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
	ASSERT(bNameValid);
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), strTitlePane1);
	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);

	//m_leftJsonEditControl.Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL, CRect(20, 120, 600, 500), this, IDC_EDIT_JSON);
	//m_rightJsonEditControl.Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL, CRect(620, 200, 600, 500), this, IDC_EDIT_JSON_RIGHT);

	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}


//void CMainFrame::OnSize(UINT nType, int cx, int cy)
//{
//	CFrameWndEx::OnSize(nType, cx, cy);
//
//	// Update the position and size of your controls here
//	if (m_leftJsonEditControl.GetSafeHwnd() != nullptr)
//	{
//		// Calculate the size and position for the JsonEditControl
//		CRect clientRect;
//		GetClientRect(&clientRect);
//
//		int jsonEditWidth = clientRect.Width() / 2; // Take up half of the width
//		int jsonEditHeight = clientRect.Height() - 180; // Adjust as needed
//
//		// Adjust the position and size of the JsonEditControl
//		m_leftJsonEditControl.MoveWindow(4, 150, jsonEditWidth, jsonEditHeight);
//	}
//
//	if (m_rightJsonEditControl.GetSafeHwnd() != nullptr)
//	{
//		// Calculate the size and position for the JsonEditControl
//		CRect clientRect;
//		GetClientRect(&clientRect);
//
//		int jsonEditWidth = clientRect.Width() / 2; // Take up half of the width
//		int jsonEditHeight = clientRect.Height() - 180; // Adjust as needed
//
//		// Adjust the position and size of the JsonEditControl
//		m_rightJsonEditControl.MoveWindow(620, 150, jsonEditWidth, jsonEditHeight);
//	}
//}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(TRUE);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

//void CMainFrame::OnOpenFirstResourceFile()
//{
//	CFileDialog fileDialog(
//		TRUE,
//		_T(".json"),
//		NULL,
//		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST,
//		_T("JSON Files (*.json)|*.json||"));
//
//	if (fileDialog.DoModal() == IDOK)
//	{
//		ResourceAPath = fileDialog.GetPathName();
//
//		CStdioFile file;
//		if (file.Open(ResourceAPath, CFile::modeRead | CFile::typeText))
//		{
//			CString jsonContent;
//			CString line;
//			while (file.ReadString(line))
//			{
//				jsonContent += line;
//			}
//			file.Close();
//
//			// Parse the JSON content manually
//			ResourceA = LoadResourceJSONFile(jsonContent);
//		}
//	}
//}
//
//void CMainFrame::OnOpenSecondResourceFile()
//{
//	CFileDialog fileDialog(
//		TRUE, 
//		_T(".json"), 
//		NULL, 
//		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, 
//		_T("JSON Files (*.json)|*.json||"));
//
//	if (fileDialog.DoModal() == IDOK)
//	{
//		CString selectedFilePath = fileDialog.GetPathName();
//
//		CStdioFile file;
//		if (file.Open(selectedFilePath, CFile::modeRead | CFile::typeText))
//		{
//			CString jsonContent;
//			CString line;
//			while (file.ReadString(line))
//			{
//				jsonContent += line;
//			}
//			file.Close();
//
//			// Parse the JSON content manually
//			ResourceB = LoadResourceJSONFile(jsonContent);
//		}
//	}
//}
//
//std::map<CString, CString> CMainFrame::LoadResourceJSONFile(CString jsonContent)
//{
//	std::map<CString, CString> keyValuePairs;
//
//	int pos = 0;
//	while (pos < jsonContent.GetLength())
//	{
//		// Find the next key
//		int keyStart = jsonContent.Find('"', pos);
//		if (keyStart == -1)
//			break;
//
//		int keyEnd = jsonContent.Find('"', keyStart + 1);
//		if (keyEnd == -1)
//			break;
//
//		CString key = jsonContent.Mid(keyStart + 1, keyEnd - keyStart - 1);
//
//		// Find the next value
//		int valueStart = jsonContent.Find('"', keyEnd + 1);
//		if (valueStart == -1)
//			break;
//
//		int valueEnd = jsonContent.Find('"', valueStart + 1);
//		if (valueEnd == -1)
//			break;
//
//		CString value = jsonContent.Mid(valueStart + 1, valueEnd - valueStart - 1);
//
//		keyValuePairs[key] = value;
//
//		pos = valueEnd + 1;
//	}
//
//	return keyValuePairs;
//}

//void CMainFrame::OnEnSetfocusJsonEditControl()
//{
//	InvalidateRightJSON();
//}
//
//void CMainFrame::InvalidateRightJSON()
//{
//	GetDlgItem(IDC_EDIT_JSON_RIGHT)->Invalidate();
//}
