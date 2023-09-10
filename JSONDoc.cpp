#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "DialogueTranslationApp.h"
#endif

#include "JSONDoc.h"
#include "JsonEdit.h"
#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <fstream>
#include <afxwin.h>
#include "MainFrm.h"

IMPLEMENT_DYNCREATE(JSONDoc, CDocument)

BEGIN_MESSAGE_MAP(JSONDoc, CDocument)
END_MESSAGE_MAP()

JSONDoc::JSONDoc()
{
}

JSONDoc::~JSONDoc()
{
}

BOOL JSONDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}

BOOL JSONDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	CStdioFile file;
	if (file.Open(lpszPathName, CFile::modeRead | CFile::typeText))
	{
		CString jsonContent;
		CString line;
		while (file.ReadString(line))
		{
			jsonContent += line;
			jsonContent += _T("\r\n"); // Add newline for formatting
		}
		file.Close();

        CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
		// Get a pointer to your CEdit control
		JsonEditControl* pEditControl = (JsonEditControl*)AfxGetMainWnd()->GetDlgItem(IDC_EDIT_JSON);
		if (pEditControl)
		{
            CString jsonWithValues = ReplaceMessageKeysWithValues(jsonContent, pMainFrame->ResourceA);
			pEditControl->SetWindowText(jsonWithValues);
		}


		JsonEditControl* pEditControlRight = (JsonEditControl*)AfxGetMainWnd()->GetDlgItem(IDC_EDIT_JSON_RIGHT);
		if (pEditControlRight)
		{
            CString jsonWithValues = ReplaceMessageKeysWithValues(jsonContent, pMainFrame->ResourceB);
			pEditControlRight->SetWindowText(jsonWithValues);
		}
	}

	return TRUE;
}

void JSONDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}

CString JSONDoc::ReplaceMessageKeysWithValues(CString jsonContent, std::map<CString, CString> resource)
{
    CStringArray lines;
    int startPos = 0;
    int endPos = 0;
    while (endPos != -1)
    {
        endPos = jsonContent.Find('\n', startPos);
        if (endPos == -1)
            lines.Add(jsonContent.Mid(startPos));
        else
            lines.Add(jsonContent.Mid(startPos, endPos - startPos));
        startPos = endPos + 1;
    }

    for (int i = 0; i < lines.GetSize(); i++)
    {
        CString line = lines.GetAt(i);

        int colonPos = line.Find(':');
        if (colonPos != -1)
        {
            CString key = line.Left(colonPos);
            CString value = line.Mid(colonPos + 1);

            key.Trim();
            value.Trim();

            key.Remove('\"');
            value.Remove('\"');

            auto it = resource.find(value);
            if (it != resource.end())
            {
                lines.SetAt(i, CString("\"") + key + CString("\": \"") + it->second + CString("\""));
            }
        }
    }

    CString modifiedJSON;
    for (int i = 0; i < lines.GetSize(); i++)
    {
        modifiedJSON += lines.GetAt(i);
        if (i < lines.GetSize() - 1)
            modifiedJSON += '\n';
    }

    return modifiedJSON;
}

#ifdef _DEBUG
void JSONDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void JSONDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG
