// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "just_BCGP.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CBCGPFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CBCGPFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_CLOSE, &CMainFrame::OnFileClose)
	ON_REGISTERED_MESSAGE(BCGM_RESETTOOLBAR, &CMainFrame::OnToolbarReset)
	ON_COMMAND(ID_TOOLBOX, &CMainFrame::OnToolBox)
	ON_MESSAGE(WM_DPICHANGED, &CMainFrame::OnDPIChanged)
	ON_COMMAND(ID_COMBO_ZOOM, OnComboZoom)
	ON_COMMAND(ID_COMBO_SNAP, OnComboSnap)
	ON_CBN_SELCHANGE(ID_COMBO_ZOOM, OnComboZoom)
	ON_CBN_EDITCHANGE(ID_COMBO_ZOOM, OnComboZoom)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
	: m_combo_zoom(ID_COMBO_ZOOM, CImageHash::GetImageOfCommand(ID_COMBO_ZOOM, FALSE), CBS_DROPDOWN, 100)
	, m_combo_snap(ID_COMBO_SNAP, CImageHash::GetImageOfCommand(ID_COMBO_SNAP, FALSE), CBS_DROPDOWN, 100)
{
	SetDockingBarsIconList(IDB_WORKSPACE);

	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

CBCGPToolbarComboBoxButton* CMainFrame::GetZoomCombo() const
{
	return
		(CBCGPToolbarComboBoxButton*)m_wndToolBarMain.GetButton
		(m_wndToolBarMain.CommandToIndex(ID_COMBO_ZOOM));
}

int CMainFrame::Create_Panel_Symbols()
{
	const int nPaneSize = globalUtils.ScaleByDPI(200, this);

	if (!m_panel_symbols.Create(_T("symbols"), this, CRect(0, 0, nPaneSize, nPaneSize),
		TRUE, ID_VIEW_PANEL_SYMBOLS,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create panel symbols\n");
		return -1;      // fail to create
	}

	return 0;
}


int CMainFrame::Create_Panel_Pages()
{
	const int nPaneSize = globalUtils.ScaleByDPI(200, this);

	if (!m_panel_pages.Create(_T("pages"), this, CRect(0, 0, nPaneSize, nPaneSize),
		TRUE, ID_VIEW_PANEL_PAGES,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create panel pages\n");
		return -1;      // fail to create
	}

	return 0;
}


int CMainFrame::Create_Panel_Layers()
{
	const int nPaneSize = globalUtils.ScaleByDPI(200, this);

	if (!m_panel_layers.Create(_T("layers"), this, CRect(0, 0, nPaneSize, nPaneSize),
		TRUE, ID_VIEW_PANEL_LAYERS,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create panel layers\n");
		return -1;      // fail to create
	}

	return 0;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CBCGPToolBar::EnableQuickCustomization();

	// Menu will not take the focus on activation:
	CBCGPPopupMenu::SetForceMenuFocus(FALSE);

	const BOOL lb_destroyed = DestroyAcceleratorTable(m_hAccelTable);
	if (lb_destroyed)
	{
		HINSTANCE hInst = AfxGetResourceHandle();
		HACCEL l_accel = LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_MAINFRAME));
		m_hAccelTable = l_accel;
		DrawMenuBar();
	}


	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetBarStyle(m_wndMenuBar.GetBarStyle() | CBRS_SIZE_DYNAMIC);

	if (!m_wndToolBarMain.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1, 1, 1, 1), ID_TOOLBAR1) ||
		!m_wndToolBarMain.LoadToolBar(IDR_MAINFRAME, 0, 0, FALSE, 0, 0, 0))
	{
		TRACE0("Failed to create toolbar Main\n");
		return -1;      // fail to create
	}


	bool lb_second_toolbar = true;
	if (lb_second_toolbar)
	{
		if (!m_wndToolBarShapes.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1, 1, 1, 1), ID_TOOLBAR2) ||
			!m_wndToolBarShapes.LoadToolBar(IDR_TOOLBAR_SHAPES, 0, 0, FALSE, 0, 0, 0))
		{
			TRACE0("Failed to create toolbar Shapes\n");
			return -1;      // fail to create
		}
	}





	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	const int nPaneSize = globalUtils.ScaleByDPI(200, this);

	if (!m_wndWorkSpace.Create(_T("View 1"), this, CRect(0, 0, nPaneSize, nPaneSize),
		TRUE, ID_VIEW_WORKSPACE,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Workspace bar\n");
		return -1;      // fail to create
	}

	m_wndWorkSpace.SetIconIndex(0);

	if (!m_wndWorkSpace2.Create(_T("View 2"),
		this, TRUE, MAKEINTRESOURCE(IDD_DIALOGBAR),
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI,
		ID_VIEW_WORKSPACE2))
	{
		TRACE0("Failed to create Workspace bar 2\n");
		return -1;      // fail to create
	}

	m_wndWorkSpace2.SetIconIndex(1);


	const int nOutputPaneSize = globalUtils.ScaleByDPI(150, this);

	if (!m_wndOutput.Create(_T("Output"), this, CSize(nOutputPaneSize, nOutputPaneSize),
		TRUE /* Has gripper */, ID_VIEW_OUTPUT,
		WS_CHILD | WS_VISIBLE | CBRS_BOTTOM))
	{
		TRACE0("Failed to create output bar\n");
		return -1;      // fail to create
	}
	m_wndOutput.SetIconIndex(2);

	if (!m_wndPropGrid.Create(_T("Vlastnosti"), this, CRect(0, 0, nPaneSize, nPaneSize),
		TRUE,
		ID_VIEW_PROPERTIES,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Properties Bar\n");
		return FALSE;		// fail to create
	}

	m_wndPropGrid.SetIconIndex(3);


	if (!m_wndToolBox.Create(_T("ToolBox"), this, CRect(0, 0, nPaneSize, 2 * nPaneSize), TRUE, ID_TOOLBOX,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_BCGP_CLOSE | CBRS_BCGP_FLOAT))
	{
		TRACE0("Failed to create ToolBox Bar\n");
		return FALSE;      // fail to create
	}

	m_wndToolBox.SetIconIndex(6);


	if(-1 == Create_Panel_Symbols())	{		return FALSE;	}
	if(-1 == Create_Panel_Pages())	{		return FALSE;	}
	if(-1 == Create_Panel_Layers())	{		return FALSE;	}


	CString strMainToolbarTitle;
	strMainToolbarTitle.LoadString(IDS_MAIN_TOOLBAR);
	m_wndToolBarMain.SetWindowText(strMainToolbarTitle);

	// TODO: delete these three lines if you don't want the toolbar to be dockable
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);

	m_wndToolBarMain.EnableDocking(CBRS_ALIGN_ANY);

	if (lb_second_toolbar)
	{
		m_wndToolBarShapes.EnableDocking(CBRS_ALIGN_ANY);
	}

	m_wndWorkSpace.EnableDocking(CBRS_ALIGN_ANY);
	m_wndWorkSpace2.EnableDocking(CBRS_ALIGN_ANY);

	m_panel_symbols.EnableDocking(CBRS_ALIGN_ANY);
	m_panel_pages.EnableDocking(CBRS_ALIGN_ANY);
	m_panel_layers.EnableDocking(CBRS_ALIGN_ANY);

	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	m_wndPropGrid.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBox.EnableDocking(CBRS_ALIGN_ANY);

	EnableDocking(CBRS_ALIGN_ANY);
	EnableAutoHideBars(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndMenuBar);

	DockControlBar(&m_wndToolBarMain);

	if (lb_second_toolbar)
	{
		DockControlBar(&m_wndToolBarShapes);
	}

	DockControlBar(&m_wndWorkSpace);
	m_wndWorkSpace2.AttachToTabWnd(&m_wndWorkSpace, BCGP_DM_STANDARD, FALSE, NULL);

	DockControlBar(&m_panel_symbols);
	DockControlBar(&m_panel_pages);
	DockControlBar(&m_panel_layers);

	DockControlBar(&m_wndOutput);
	DockControlBar(&m_wndPropGrid);
	DockControlBar(&m_wndToolBox);
	m_wndToolBox.SetAutoHideMode(TRUE, CBRS_ALIGN_RIGHT);

	return 0;
}



void CMainFrame::ReloadPanelPages(Cjust_BCGPDoc* pDoc)
{
	m_panel_pages.ReloadFromDoc(pDoc);
}



BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CBCGPFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CBCGPFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CBCGPFrameWnd::Dump(dc);
}

#endif //_DEBUG

// CMainFrame message handlers

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext)
{
	// base class does the real work

	if (!CBCGPFrameWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}

	CWinApp* pApp = AfxGetApp();
	if (pApp->m_pMainWnd == NULL)
		pApp->m_pMainWnd = this;

	return TRUE;
}

void CMainFrame::OnFileClose()
{
   DestroyWindow();
}

LRESULT CMainFrame::OnToolbarReset(WPARAM wp,LPARAM)
{
	// TODO: reset toolbar with id = (UINT) wp to its initial state:
	//
	UINT uiToolBarId = (UINT) wp;
	if (uiToolBarId == IDR_MAINFRAME)
	{
		Create_Combo_Snap();
		Create_Combo_Zoom();
		
	}

	return 0;
}

void CMainFrame::OnToolBox()
{
	int nPage = -1;
	int nTool = m_wndToolBox.GetLastClickedTool(nPage);

	const MSG* pMsg = CWnd::GetCurrentMessage();

	if (pMsg->lParam != (LPARAM)m_wndToolBox.GetSafeHwnd())
	{
		m_wndToolBox.ShowControlBar(!m_wndToolBox.IsVisible(), FALSE, TRUE);
		return;
	}

	TRACE(_T("Toolbox item clicked page = %d, item = %d\n"), nPage, nTool);
}



void CMainFrame::OnComboZoom()
{
	CBCGPToolbarComboBoxButton* pCombo = GetZoomCombo();
	if (pCombo)
	{
		CEdit* pEdit = pCombo->GetEditCtrl();//clear the registry if it crashes here
		CString ls_what;
		pEdit->GetWindowText(ls_what);
	}
}


void CMainFrame::OnComboSnap()
{
	int i = 5;
}

void CMainFrame::Set_Theme_Dark_Or_Light(bool ab_dark)
{
	CBCGPToolBar::ResetAllImages(FALSE /* Don't reset size */);

	if (ab_dark)
	{
		m_wndToolBarMain.LoadBitmap(IDR_MAINFRAME_DARK);
	}
	else
	{
		m_wndToolBarMain.LoadBitmap(IDR_MAINFRAME);
	}

	RecalcLayout();
	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_FRAME | RDW_UPDATENOW | RDW_ALLCHILDREN);
}

LRESULT CMainFrame::OnDPIChanged(WPARAM wp, LPARAM lp)
{
	LRESULT lRes = CBCGPFrameWnd::OnDPIChanged(wp, lp);

	return lRes;
}

void CMainFrame::Create_Combo_Zoom()
{
	CBCGPToolbarComboBoxButton comboButton(ID_COMBO_ZOOM,
		CImageHash::GetImageOfCommand(ID_COMBO_ZOOM, FALSE),
		CBS_DROPDOWNLIST, 100);

	m_combo_zoom.AddItem(L"  10%");
	m_combo_zoom.AddItem(L"  25%");
	m_combo_zoom.AddItem(L"  35%");
	m_combo_zoom.AddItem(L"  50%");
	m_combo_zoom.AddItem(L"  75%");
	m_combo_zoom.AddItem(L" 100%");
	m_combo_zoom.AddItem(L" 150%");
	m_combo_zoom.AddItem(L" 200%");
	m_combo_zoom.AddItem(L" 300%");
	m_combo_zoom.AddItem(L" 500%");
	m_combo_zoom.AddItem(L"1000%");
	m_combo_zoom.AddItem(L"3000%");


	int li_count = m_wndToolBarMain.ReplaceButton(ID_COMBO_ZOOM, m_combo_zoom);
}

void CMainFrame::Create_Combo_Snap()
{
	CBCGPToolbarComboBoxButton comboButton(ID_COMBO_SNAP,
		CImageHash::GetImageOfCommand(ID_COMBO_SNAP, FALSE),
		CBS_DROPDOWNLIST, 100);




	m_combo_snap.AddItem(L" 0.2 mm");
	m_combo_snap.AddItem(L" 0.5 mm");
	m_combo_snap.AddItem(L" 1.0 mm");
	m_combo_snap.AddItem(L" 2.0 mm");
	m_combo_snap.AddItem(L" 4.0 mm");
	m_combo_snap.AddItem(L" 5.0 mm");
	m_combo_snap.AddItem(L"10.0 mm");
	m_combo_snap.AddItem(L"20.0 mm");

	m_combo_snap.AddItem(L"1/10 \"");
	m_combo_snap.AddItem(L"1/5 \"");
	m_combo_snap.AddItem(L"1/4 \"");
	m_combo_snap.AddItem(L"1/3 \"");
	m_combo_snap.AddItem(L"1/2 \"");
	m_combo_snap.AddItem(L"1 \"");


	int li_count = m_wndToolBarMain.ReplaceButton(ID_COMBO_SNAP, m_combo_snap);

}

