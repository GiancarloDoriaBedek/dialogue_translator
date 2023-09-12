#include "stdafx.h"
#include "resource.h"
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
#include "MainView.h"

IMPLEMENT_DYNCREATE(JSONDoc, CDocument)

BEGIN_MESSAGE_MAP(JSONDoc, CDocument)
ON_EN_CHANGE(IDC_EDIT_JSON, &JSONDoc::OnEnChange)
ON_COMMAND(ID_FILE_SAVE, &JSONDoc::OnFileSave)
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

	//CStdioFile file;
	//if (file.Open(lpszPathName, CFile::modeRead | CFile::typeText))
	//{
	//	CString line;
	//	while (file.ReadString(line))
	//	{
 //           JSONContent += line;
 //           JSONContent += _T("\r\n"); // Add newline for formatting
	//	}
	//	file.Close();

		// Only left json is editable
		/*JsonEditControl* pEditControl = (JsonEditControl*)AfxGetMainWnd()->GetDlgItem(IDC_EDIT_JSON);
		if (pEditControl)
		{
            CString jsonWithValues = ReplaceMessageKeysWithValues(JSONContent, pEditControl->ResourceA);
            m_jsonDialogueOriginal = jsonWithValues;
			pEditControl->SetWindowText(jsonWithValues);
		}


		JsonEditControl* pEditControlRight = (JsonEditControl*)AfxGetMainWnd()->GetDlgItem(IDC_EDIT_JSON_RIGHT);
		if (pEditControlRight)
		{
            CString jsonWithValues = ReplaceMessageKeysWithValues(JSONContent, pMainFrame->ResourceB);
			pEditControlRight->SetWindowText(jsonWithValues);
		}*/
	//}

    CStdioFile file;
    if (file.Open(lpszPathName, CFile::modeRead | CFile::typeText))
    {
        CString line;
        while (file.ReadString(line))
        {
            m_jsonDialogueOriginal += line;
            m_jsonDialogueOriginal += _T("\r\n"); // Add newline for formatting
        }
        file.Close();
    }


    CString myString;
    
    UpdateAllViews(nullptr, 0, (CObject*)&JSONContent);

	return TRUE;
}

CString JSONDoc::ReplaceMessageKeysWithValues(std::map<CString, CString> resource)
{
    CStringArray lines;
    int startPos = 0;
    int endPos = 0;
    while (endPos != -1)
    {
        endPos = m_jsonDialogueOriginal.Find('\n', startPos);
        if (endPos == -1)
            lines.Add(m_jsonDialogueOriginal.Mid(startPos));
        else
            lines.Add(m_jsonDialogueOriginal.Mid(startPos, endPos - startPos));
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

void JSONDoc::OnEnChange()
{
    CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
    JsonEditControl* pEditControl = (JsonEditControl*)AfxGetMainWnd()->GetDlgItem(IDC_EDIT_JSON);

    if (pEditControl)
    {
        CString editedText;
        pEditControl->GetWindowText(editedText);

        CStringArray originalLines;
        int originalStartPos = 0;
        int originalEndPos = 0;
        while (originalEndPos != -1)
        {
            originalEndPos = m_jsonDialogueOriginal.Find(_T("\n"), originalStartPos);
            if (originalEndPos == -1)
                originalLines.Add(m_jsonDialogueOriginal.Mid(originalStartPos));
            else
                originalLines.Add(m_jsonDialogueOriginal.Mid(originalStartPos, originalEndPos - originalStartPos));
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
            CString originalMessageField;
            int messageFieldStart = originalLine.Find(_T("\"message\":"));
            if (messageFieldStart != -1)
            {
                int valueStart = originalLine.Find(_T("\""), messageFieldStart + 11);
                if (valueStart != -1)
                {
                    int valueEnd = originalLine.Find(_T("\""), valueStart + 1);
                    if (valueEnd != -1)
                    {
                        originalMessageField = originalLine.Mid(valueStart + 1, valueEnd - valueStart - 1);
                    }
                }
            }

            CString editedMessageField;
            if (messageFieldStart != -1)
            {
                int valueStart = editedLine.Find(_T("\""), messageFieldStart + 11);
                if (valueStart != -1)
                {
                    int valueEnd = editedLine.Find(_T("\""), valueStart + 1);
                    if (valueEnd != -1)
                    {
                        editedMessageField = editedLine.Mid(valueStart + 1, valueEnd - valueStart - 1);
                    }
                }
            }

            //ReplaceInMap(originalMessageField, editedMessageField);
            m_jsonDialogueOriginal = editedText;
            CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
            //pMainFrame->ResourceA;
        }
    }

    //pMainFrame->InvalidateRightJSON();
}

//void JSONDoc::ReplaceInMap(const CString& originalValue, const CString& changedValue)
//{
//    CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
//
//    CString key;
//
//    for (const auto& pair : pMainFrame->ResourceA)
//    {
//        if (pair.second == originalValue)
//        {
//            pMainFrame->ResourceA[pair.first] = changedValue;
//            return;
//        }
//    }
//}

//void JSONDoc::OnFileSave()
//{
//    CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
//
//    CFile file;
//    if (!file.Open(pMainFrame->ResourceAPath, CFile::modeCreate | CFile::modeWrite))
//    {
//        AfxMessageBox(_T("Failed to open the file for writing."));
//        return;
//    }
//
//    // Create a JSON-like string from the map
//    CString jsonContent = _T("{\n");
//    for (const auto& pair : pMainFrame->ResourceA) {
//        jsonContent += _T("  \"") + pair.first + _T("\": \"") + pair.second + _T("\",\n");
//    }
//    // Remove the trailing comma and newline
//    jsonContent = jsonContent.Left(jsonContent.GetLength() - 2) + _T("\n}");
//
//    // Write the string to the file
//    file.Write(jsonContent, jsonContent.GetLength() * sizeof(TCHAR));
//    file.Close();
//}


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
