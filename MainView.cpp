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
	ON_EN_CHANGE(IDC_EDIT_JSON, &MainView::OnEnChange)
	ON_COMMAND(ID_BUTTON_FIRST, &MainView::OnOpenFirstResourceFile)
	ON_COMMAND(ID_BUTTON_SECOND, &MainView::OnOpenSecondResourceFile)
	ON_COMMAND(ID_FILE_SAVE, &MainView::OnFileSave)
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

}

int MainView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CView::OnCreate(lpCreateStruct);
	lpCreateStruct->cx = 520;
	lpCreateStruct->cy = 1230;
	m_leftJsonEditControl.Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL, CRect(10, 10, 610, 500), this, IDC_EDIT_JSON);
	m_rightJsonEditControl.Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL, CRect(620, 10, 1220, 500), this, IDC_EDIT_JSON_RIGHT);

	return 0;
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
}

LRESULT MainView::OnUpdateLeftJson(WPARAM wParam, LPARAM lParam)
{
	JSONDoc* pDoc = GetDocument();

	//Only left json is editable
	CString leftJsonWithValues = pDoc->ReplaceMessageKeysWithValues(ResourceA);
	pDoc->JSONContent = leftJsonWithValues;
	m_leftJsonEditControl.SetWindowText(leftJsonWithValues);
	

	CString rightJsonWithValues = pDoc->ReplaceMessageKeysWithValues(ResourceB);
	m_rightJsonEditControl.SetWindowText(rightJsonWithValues);
	m_rightJsonEditControl.Invalidate();
	return 0;
}

/**
 * Handles changes in the text content of the JSON editor controls.
 * Compares the edited JSON lines with the original lines and updates the documents content accordingly.
 */
void MainView::OnEnChange()
{
	JSONDoc* pDoc = GetDocument();

	CString editedText;
	m_leftJsonEditControl.GetWindowTextW(editedText);
	CStringArray originalLines;
	int originalStartPos = 0;
	int originalEndPos = 0;
	while (originalEndPos != -1)
	{
		originalEndPos = pDoc->JSONContent.Find(_T("\n"), originalStartPos);
		if (originalEndPos == -1)
			originalLines.Add(pDoc->JSONContent.Mid(originalStartPos));
		else
			originalLines.Add(pDoc->JSONContent.Mid(originalStartPos, originalEndPos - originalStartPos));
		originalStartPos = originalEndPos + 1;
	}

	CStringArray editedLines;
	int startPos = 0;
	int endPos = 0;
	while (endPos != -1)
	{
		endPos = editedText.Find(_T("\n"), startPos);
		if (endPos == -1)
			editedLines.Add(editedText.Mid(startPos));
		else
			editedLines.Add(editedText.Mid(startPos, endPos - startPos));
		startPos = endPos + 1;
	}

	for (int i = 0; i < editedLines.GetSize(); i++)
	{
		CString editedLine = editedLines.GetAt(i);
		CString originalLine = (i < originalLines.GetSize()) ? originalLines.GetAt(i) : _T("");

		if (originalLine == editedLine)
		{
			continue;
		}

		int messageFieldStart = originalLine.Find(_T("\"message\""));
		if (messageFieldStart == -1)
		{
			continue;
		}

		CString originalMessageField = FindMessageValue(originalLine);
		CString editedMessageField = FindMessageValue(editedLine);
		
		ReplaceInMap(originalMessageField, editedMessageField);
		pDoc->JSONContent = editedText;
	}
}

/**
 * Finds and extracts the value associated with a "message" field in a JSON line.
 *
 * @param line The JSON line to search for the "message" field.
 * @return The value associated with the "message" field, or an empty string if not found.
 */
CString MainView::FindMessageValue(CString line)
{
	CString messageFieldValue;
	int messageFieldStart = line.Find(_T("\"message\""));
	int colonPos = line.Find(_T(":"), messageFieldStart + 9);

	if (colonPos != -1)
	{
		int valueStart = line.Find(_T("\""), colonPos + 1);
		if (valueStart != -1)
		{
			int valueEnd = line.Find(_T("\""), valueStart + 1);
			if (valueEnd != -1)
			{
				messageFieldValue = line.Mid(valueStart + 1, valueEnd - valueStart - 1);
			}
		}
	}

	return messageFieldValue;
}

/**
 * Replaces a value in the resource map (ResourceA) with a changed value.
 * Searches for the original value within the map and updates it with the new value.
 *
 * @param originalValue The original value to be replaced.
 * @param changedValue The new value to replace the original one.
 */
void MainView::ReplaceInMap(const CString& originalValue, const CString& changedValue)
{
    CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());

    CString key;

    for (const auto& pair : ResourceA)
    {
        if (pair.second == originalValue)
        {
            ResourceA[pair.first] = changedValue;
            return;
        }
    }
}

void MainView::OnFileSave()
{
    CFile file;
    if (!file.Open(ResourceAPath, CFile::modeCreate | CFile::modeWrite))
    {
        AfxMessageBox(_T("Failed to open the file for writing."));
        return;
    }

    // Create a JSON-like string from the map
    CString jsonContent = _T("{\n");
    for (const auto& pair : ResourceA) {
        jsonContent += _T("  \"") + pair.first + _T("\": \"") + pair.second + _T("\",\n");
    }
    // Remove the trailing comma and newline
    jsonContent = jsonContent.Left(jsonContent.GetLength() - 2) + _T("\n}");

    // Write the string to the file
    file.Write(jsonContent, jsonContent.GetLength() * sizeof(TCHAR));
    file.Close();
}