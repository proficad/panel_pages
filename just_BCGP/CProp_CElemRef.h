#pragma once
#include <BCGPPropList.h>

enum PropertyType;


class CProp_CElemRef : public CBCGPProp
{
public:
	CProp_CElemRef(const CString& strName, const _variant_t& varValue, DWORD_PTR 	dwData = 0);

	virtual BOOL OnUpdateValue();
	virtual BOOL OnEndEdit();

};

