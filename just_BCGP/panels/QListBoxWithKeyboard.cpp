// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// QListBoxWithKeyboard.cpp : implementation file
//

#include "stdafx.h"
#include "QListBoxWithKeyboard.h"
#include "../resource.h"

// QListBoxWithKeyboard

IMPLEMENT_DYNAMIC(QListBoxWithKeyboard, CBCGPDragListBox)

QListBoxWithKeyboard::QListBoxWithKeyboard()
{
//	bitmapBkgnd.Attach (LoadBitmap (AfxGetInstanceHandle(), MAKEINTRESOURCE("IDR_BG1")));
}

QListBoxWithKeyboard::~QListBoxWithKeyboard()
{
}


BEGIN_MESSAGE_MAP(QListBoxWithKeyboard, CBCGPDragListBox)
	ON_WM_KEYDOWN()
//	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// QListBoxWithKeyboard message handlers
/*
BOOL QListBoxWithKeyboard::OnEraseBkgnd(CDC* pDC) 
{
	CRect rect;
	GetClientRect(&rect);
	CDC dc;
	dc.CreateCompatibleDC(pDC);
	CBitmap* pOldBitmap = dc.SelectObject(&bitmapBkgnd);
	int iBitmapWidth, iBitmapHeight ;
	int ixOrg, iyOrg;

	BITMAP bm;
	bitmapBkgnd.GetObject(sizeof(BITMAP),&bm);
	iBitmapWidth = bm.bmWidth;
	iBitmapHeight = bm.bmHeight;

	// If our bitmap is smaller than the background and tiling is
	// supported, tile it. Otherwise watch the efficiency - don't
	// spend time setting up loops you won't need.

	if (iBitmapWidth  >= rect.Width() &&
		iBitmapHeight >= rect.Height() )
	{
		pDC->BitBlt (rect.left, 
			rect.top, 
			rect.Width(),
			rect.Height(),
			&dc,
			0, 0, SRCCOPY);
	}
	else
	{
		for (iyOrg = 0; iyOrg < rect.Height(); iyOrg += iBitmapHeight)
		{
			for (ixOrg = 0; ixOrg < rect.Width(); ixOrg += iBitmapWidth)
			{
				pDC->BitBlt (ixOrg, 
					iyOrg, 
					rect.Width(),
					rect.Height(),
					&dc,
					0, 0, SRCCOPY);
			}
		}
	}

	dc.SelectObject(pOldBitmap);
	return TRUE;
}
*/

void QListBoxWithKeyboard::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{

	if (nChar == VK_DELETE)
	{
		CWnd* pWnd = this->GetParent();
		if (pWnd)
		{
			int li_currentItem = GetCurSel();
			if(LB_ERR != li_currentItem)
			{
				pWnd->SendMessage(UWM_DELETE_SELECTEDITEM, li_currentItem, 1);
			}
		}
		
	}

	
	CListBox::OnKeyDown(nChar, nRepCnt, nFlags);
}


void QListBoxWithKeyboard::Dropped( int nSrcIndex, CPoint pt )
{
	int li_what = nSrcIndex;
	int li_target = m_nLast;

	LB_DROPPED l_dropped;
	l_dropped.hdr.hwndFrom = m_hWnd;
	l_dropped.hdr.idFrom   = GetDlgCtrlID();
	l_dropped.hdr.code     = LISTBOX_DROPPED;
	l_dropped.iSource = li_what;
	l_dropped.iTarget = li_target;

	CWnd* pWnd = this->GetParent();
	if (pWnd)
	{
		pWnd->SendMessage(WM_NOTIFY, 0, (LPARAM)&l_dropped);
	}

	CBCGPDragListBox::Dropped(nSrcIndex, pt);


}
