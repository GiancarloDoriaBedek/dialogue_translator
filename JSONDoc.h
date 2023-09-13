#pragma once
#include <map>

class JSONDoc : public CDocument
{
protected: // create from serialization only
	DECLARE_DYNCREATE(JSONDoc)

// Attributes
public:
	JSONDoc();

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	CString JSONContent;
	CString ReplaceMessageKeysWithValues(std::map<CString, CString> resource);
	
	CString m_jsonDialogueOriginal;

private:
	std::map<CString, std::map<CString, CString>> m_mappedMessages;

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
