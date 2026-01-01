// MainFrm.h : interface of the CMainFrame class
//


#pragma once
#include "WorkSpaceBar.h"
#include "WorkSpaceBar2.h"
#include "OutputBar.h"
#include "PropertiesViewBar.h"
#include "ToolBox.h"
#include "panels/Panel_Layers.h"
#include "panels/Panel_Pages.h"
#include "panels/Panel_Symbols.h"

class CMainFrame : public CBCGPFrameWnd
{

protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:
	void ReloadPanelPages(Cjust_BCGPDoc* pDoc);


// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

	void Set_Theme_Dark_Or_Light(bool ab_dark);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CBCGPStatusBar			m_wndStatusBar;
	CBCGPMenuBar			m_wndMenuBar;

	CBCGPToolBar			m_wndToolBarMain;
	CBCGPToolBar			m_wndToolBarShapes;


	CWorkSpaceBar			m_wndWorkSpace;
	CWorkSpaceBar2			m_wndWorkSpace2;

	Panel_Symbols			m_panel_symbols;
	Panel_Pages				m_panel_pages;
	Panel_Layers			m_panel_layers;

	COutputBar				m_wndOutput;
	CBCGPPropBar			m_wndPropGrid;
	CToolBoxBarEx			m_wndToolBox;


	CBCGPToolbarComboBoxButton* GetZoomCombo() const;

	int Create_Panel_Symbols();
	int Create_Panel_Pages();
	int Create_Panel_Layers();

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFileClose();
	afx_msg LRESULT OnToolbarReset(WPARAM,LPARAM);
	afx_msg void OnToolBox();

	afx_msg void OnComboZoom();
	afx_msg void OnComboSnap();

	afx_msg LRESULT OnDPIChanged(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()

private:

	CBCGPToolbarComboBoxButton m_combo_zoom;
	CBCGPToolbarComboBoxButton m_combo_snap;


	void Create_Combo_Zoom();
	void Create_Combo_Snap();


};
