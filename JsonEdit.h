#pragma once

#include "resource.h"
#include <afxwin.h>

class JsonEditControl : public CEdit
{
public:
    JsonEditControl();

protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnEnChange();
    DECLARE_MESSAGE_MAP()

private:
    CString m_jsonContent;
};
