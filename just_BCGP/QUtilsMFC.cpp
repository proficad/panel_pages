#include "stdafx.h"
#include "QUtilsMFC.h"

Cjust_BCGPDoc* QUtilsMFC::GetActiveDoc()
{
	CView* pView = GetActiveView();
	if (pView)
	{
		return  dynamic_cast<Cjust_BCGPDoc*>(pView->GetDocument());
	}

	return nullptr;
}

CView* QUtilsMFC::GetActiveView()
{
	CMDIFrameWnd* pFrame =
		(CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;

	if (!pFrame) {
		return NULL;
	}
	// Get the active MDI child window.
	CMDIChildWnd* pChild =
		(CMDIChildWnd*)pFrame->GetActiveFrame();

	if (!pChild) {
		return NULL;
	}

	// or CMDIChildWnd *pChild = pFrame->MDIGetActive();

	// Get the active view attached to the active MDI child
	// window.
	return (CView*)pChild->GetActiveView();
}
