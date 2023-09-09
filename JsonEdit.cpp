// MyDialog.cpp
#include "stdafx.h"
#include "JsonEdit.h"
#include <afxwin.h>

BEGIN_MESSAGE_MAP(JsonEditControl, CEdit)
    ON_WM_CREATE()
    ON_EN_CHANGE(IDC_EDIT_JSON, &JsonEditControl::OnEnChange)
END_MESSAGE_MAP()

JsonEditControl::JsonEditControl()
{

}

int JsonEditControl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CEdit::OnCreate(lpCreateStruct) == -1)
        return -1;

    SetWindowText(m_jsonContent); // Set JSON content in the edit control

    return 0;
}

void JsonEditControl::OnEnChange()
{
    GetWindowText(m_jsonContent); // Update JSON content when text changes
}