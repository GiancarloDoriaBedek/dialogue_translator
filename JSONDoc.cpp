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

/**
 * Replaces message keys with corresponding values from a resource map in the JSONcontent.
 * Parses and modifies the JSON content line by line, replacing values based on matching keys.
 *
 * @param resourceA map containing key-value pairs for replacement.
 * @return The modified JSON content with replaced values.
 */
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
