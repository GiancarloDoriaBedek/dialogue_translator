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
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_BUTTON_FIRST, &MainView::OnOpenFirstResourceFile)
	ON_COMMAND(ID_BUTTON_SECOND, &MainView::OnOpenSecondResourceFile)
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

void MainView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	// Document = GetDocument();

}

int MainView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	m_leftJsonEditControl = new JsonEditControl();
	m_leftJsonEditControl->Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL, CRect(20, 120, 600, 500), this, IDC_EDIT_JSON);
	m_rightJsonEditControl = new JsonEditControl();
	m_rightJsonEditControl->Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL, CRect(620, 200, 600, 500), this, IDC_EDIT_JSON_RIGHT);

	return 0;
}

// MainView drawing

void MainView::OnDraw(CDC* pDC)
{
	//// Create a font and set it for drawing
	//CFont font;
	//font.CreatePointFont(200, _T("Arial"));
	//pDC->SelectObject(&font);

	//// Draw text on the view
	//pDC->TextOut(100, 100, _T("Hello, MFC!"));
	return;
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

	//CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	//return (JSONDoc*)pFrame->GetActiveDocument();
}
#endif //_DEBUG


void MainView::OnOpenFirstResourceFile()
{
	CFileDialog fileDialog(
		TRUE,
		_T(".json"),
		NULL,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST,
		_T("JSON Files (*.json)|*.json||"));

	if (fileDialog.DoModal() == IDOK)
	{
		ResourceAPath = fileDialog.GetPathName();

		CStdioFile file;
		if (file.Open(ResourceAPath, CFile::modeRead | CFile::typeText))
		{
			CString jsonContent;
			CString line;
			while (file.ReadString(line))
			{
				jsonContent += line;
			}
			file.Close();

			// Parse the JSON content manually
			ResourceA = LoadResourceJSONFile(jsonContent);
		}
	}
}

void MainView::OnOpenSecondResourceFile()
{
	CFileDialog fileDialog(
		TRUE,
		_T(".json"),
		NULL,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST,
		_T("JSON Files (*.json)|*.json||"));

	if (fileDialog.DoModal() == IDOK)
	{
		CString selectedFilePath = fileDialog.GetPathName();

		CStdioFile file;
		if (file.Open(selectedFilePath, CFile::modeRead | CFile::typeText))
		{
			CString jsonContent;
			CString line;
			while (file.ReadString(line))
			{
				jsonContent += line;
			}
			file.Close();

			// Parse the JSON content manually
			ResourceB = LoadResourceJSONFile(jsonContent);
		}
	}
}

std::map<CString, CString> MainView::LoadResourceJSONFile(CString jsonContent)
{
	std::map<CString, CString> keyValuePairs;

	int pos = 0;
	while (pos < jsonContent.GetLength())
	{
		// Find the next key
		int keyStart = jsonContent.Find('"', pos);
		if (keyStart == -1)
			break;

		int keyEnd = jsonContent.Find('"', keyStart + 1);
		if (keyEnd == -1)
			break;

		CString key = jsonContent.Mid(keyStart + 1, keyEnd - keyStart - 1);

		// Find the next value
		int valueStart = jsonContent.Find('"', keyEnd + 1);
		if (valueStart == -1)
			break;

		int valueEnd = jsonContent.Find('"', valueStart + 1);
		if (valueEnd == -1)
			break;

		CString value = jsonContent.Mid(valueStart + 1, valueEnd - valueStart - 1);

		keyValuePairs[key] = value;

		pos = valueEnd + 1;
	}

	return keyValuePairs;
}


void MainView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	OnUpdateLeftJson(lHint, lHint);
	JSONDoc* pDoc = static_cast<JSONDoc*>(pHint);
}

LRESULT MainView::OnUpdateLeftJson(WPARAM wParam, LPARAM lParam)
{
	// Handle the custom message for the left JSON
	// You can update the left view based on the event

	// For example, you can call a function to refresh the left view
	JSONDoc* pDoc = GetDocument();

	 //Only left json is editable
	CString jsonWithValues = pDoc->ReplaceMessageKeysWithValues(ResourceA);
	pDoc->JSONContent = jsonWithValues;
	m_leftJsonEditControl->SetWindowText(jsonWithValues);
	

	jsonWithValues = pDoc->ReplaceMessageKeysWithValues(ResourceB);
	m_rightJsonEditControl->SetWindowText(jsonWithValues);
	
	return 0;
}

LRESULT MainView::OnUpdateRightJson(WPARAM wParam, LPARAM lParam)
{
	// Handle the custom message for the left JSON
	// You can update the left view based on the event

	// For example, you can call a function to refresh the left view

	return 0;
}