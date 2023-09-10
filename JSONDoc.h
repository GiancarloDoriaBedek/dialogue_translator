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
	virtual afx_msg void OnFileSave();
	virtual void Serialize(CArchive& ar);

private:
	CString m_jsonDialogueOriginal;
	CString m_jsonDialogueResourceA;
	CString m_jsonDialogueResourceB;
	std::map<CString, std::map<CString, CString>> m_mappedMessages;
	CString ReplaceMessageKeysWithValues(CString jsonContent, std::map<CString, CString> resource);
	void ReplaceInMap(const CString& originalValue, const CString& changedValue);


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
	afx_msg void OnEnChange();
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
#endif // SHARED_HANDLERS
};
