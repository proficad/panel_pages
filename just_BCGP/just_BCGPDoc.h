// just_BCGPDoc.h : interface of the Cjust_BCGPDoc class
//


#pragma once
#include <vector>
#include "QOnePage.h"


class QOnePage;

class Cjust_BCGPDoc : public CDocument
{
protected: // create from serialization only
	Cjust_BCGPDoc();
	DECLARE_DYNCREATE(Cjust_BCGPDoc)

// Attributes
public:
	void Move_Page(int ai_source, int ai_target);
// Operations
public:
	std::vector<QOnePage> m_pagesPrivate;
// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~Cjust_BCGPDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	int m_currentPage;
// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


