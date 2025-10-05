// just_BCGPView.h : interface of the Cjust_BCGPView class
//


#pragma once


class Cjust_BCGPView : public CView
{
protected: // create from serialization only
	Cjust_BCGPView();
	DECLARE_DYNCREATE(Cjust_BCGPView)

// Attributes
public:
	Cjust_BCGPDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void OnLButtonDblClk(UINT nFlags, CPoint point);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~Cjust_BCGPView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg void OnFilePrintPreview();
	afx_msg void OnSouborExportovat();
	afx_msg LRESULT OnPrintClient(WPARAM wp, LPARAM lp);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnViewAttributes();
	afx_msg void OnViewListBox();
	afx_msg void OnViewOwnerDraw();



};

#ifndef _DEBUG  // debug version in just_BCGPView.cpp
inline Cjust_BCGPDoc* Cjust_BCGPView::GetDocument() const
	{ return reinterpret_cast<Cjust_BCGPDoc*>(m_pDocument); }
#endif

