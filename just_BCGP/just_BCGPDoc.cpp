// just_BCGPDoc.cpp : implementation of the Cjust_BCGPDoc class
//

#include "stdafx.h"
#include "just_BCGP.h"

#include "just_BCGPDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cjust_BCGPDoc

IMPLEMENT_DYNCREATE(Cjust_BCGPDoc, CDocument)

BEGIN_MESSAGE_MAP(Cjust_BCGPDoc, CDocument)
END_MESSAGE_MAP()




Cjust_BCGPDoc::Cjust_BCGPDoc()
{
	m_pagesPrivate.push_back(QOnePage{ 1, _T("one") });
	m_pagesPrivate.push_back(QOnePage{ 2, _T("two") });
	m_pagesPrivate.push_back(QOnePage{ 3, _T("three") });
	m_pagesPrivate.push_back(QOnePage{ 4, _T("four") });	
}

Cjust_BCGPDoc::~Cjust_BCGPDoc()
{
}

BOOL Cjust_BCGPDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

// Cjust_BCGPDoc serialization

void Cjust_BCGPDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

void Cjust_BCGPDoc::Move_Page(int ai_source, int ai_target)
{
	const int li_size = static_cast<int>(m_pagesPrivate.size());
	if (li_size <= 0) return;

	if (ai_source < 0 || ai_source >= li_size) return;

	// ai_target je INSERT POSITION: povol i "na konec"
	if (ai_target < 0) ai_target = 0;
	if (ai_target > li_size) ai_target = li_size;

	// Pokud pouštíš na tu samou "mezeru", nic se nemění
	if (ai_target == ai_source || ai_target == ai_source + 1)
	{
		m_currentPage = ai_source;
		return;
	}

	if (ai_source < ai_target)
	{
		// příklad: 1 2 3 4 5, source=0, target=4  => 2 3 4 1 5
		std::rotate(m_pagesPrivate.begin() + ai_source,
			m_pagesPrivate.begin() + ai_source + 1,
			m_pagesPrivate.begin() + ai_target);

		m_currentPage = ai_target - 1;
	}
	else
	{
		// příklad: 1 2 3 4 5, source=4, target=1  => 1 5 2 3 4
		std::rotate(m_pagesPrivate.begin() + ai_target,
			m_pagesPrivate.begin() + ai_source,
			m_pagesPrivate.begin() + ai_source + 1);

		m_currentPage = ai_target;
	}

}

// Cjust_BCGPDoc diagnostics

#ifdef _DEBUG
void Cjust_BCGPDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void Cjust_BCGPDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Cjust_BCGPDoc commands
