#pragma once
#include "just_BCGPDoc.h"

class QUtilsMFC
{
public:
	static Cjust_BCGPDoc* GetActiveDoc(void);
	static CView* GetActiveView();
};

