#include "stdafx.h"
#include "CProp_CElemRef.h"


CProp_CElemRef::CProp_CElemRef(const CString& strName, const _variant_t& varValue, DWORD_PTR dwData)
	: CBCGPProp(strName, varValue, nullptr, dwData, nullptr, nullptr, nullptr)
{
}


//*******************************************************************************
BOOL CProp_CElemRef::OnUpdateValue()
{
	ASSERT_VALID(this);
	ASSERT_VALID(m_pWndInPlace);
	ASSERT_VALID(m_pWndList);
	ASSERT(::IsWindow(m_pWndInPlace->GetSafeHwnd()));

	CString strText;
	m_pWndInPlace->GetWindowText(strText);


	// check whether strText contains invalid characters like \ / : * ? " < > |
	if (strText.FindOneOf(_T("\\/:*?\"<>|")) != -1)
	{
		SetState(_T("The value contains invalid characters"));
		return FALSE;
	}

	CleanState();
	return CBCGPProp::OnUpdateValue();
}

//*******************************************************************************

BOOL CProp_CElemRef::OnEndEdit()
{
	CleanState();
	return CBCGPProp::OnEndEdit();
}



