// workspacebar.h : interface of the Panel_Pages class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Panel_With_Preview.h"
#include "QListBoxWithKeyboard.h"



class Cjust_BCGPDoc;

class Toolbar_Pages : public CBCGPToolBar
{
public:
	// All commands should be routed via owner window, not via frame:
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CBCGPToolBar::OnUpdateCmdUI((CFrameWnd*)GetOwner(), bDisableIfNoHndler);
	}

	// Toolbar should't appear on the customization list
	virtual BOOL AllowShowOnList() const { return FALSE; }
	virtual void AdjustLayout();
};


class Panel_Pages : public QPanel_With_Preview
{
public:
	Panel_Pages();
	virtual void AdjustLayout();
	void ShowPreview(int ai_index) override;

// Attributes
protected:
	Toolbar_Pages m_wndToolBar;
	QListBoxWithKeyboard m_list;
	CFont m_listFontLarge; // Member variable to hold the larger font

	void Create_Toolbar();

// Operations
public:

// Overrides

// Implementation
public:
	virtual ~Panel_Pages();
	void ReloadFromDoc(Cjust_BCGPDoc* pDoc);

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

	void Clear();
	void OnDropped(NMHDR* /*pNotifyStruct*/, LRESULT* /*result*/);

};

/////////////////////////////////////////////////////////////////////////////
