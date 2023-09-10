#pragma once
#include <map>

class JSONDoc : public CDocument
{
protected: // create from serialization only
	JSONDoc();
	DECLARE_DYNCREATE(JSONDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void Serialize(CArchive& ar);

private:
	CString m_jsonDialogueOriginal;
	CString m_jsonDialogueResourceA;
	CString m_jsonDialogueResourceB;
	CString ReplaceMessageKeysWithValues(CString jsonContent, std::map<CString, CString> resource);

// Implementation
public:
	virtual ~JSONDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
#endif // SHARED_HANDLERS
};
