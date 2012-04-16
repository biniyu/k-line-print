#include "StdAfx.h"
#include "TickRenderer.h"

TickRenderer::TickRenderer(void)
{
	memset(&m_tick, 0, sizeof(m_tick));
	m_bSelected = false;
}

TickRenderer::~TickRenderer(void)
{
}

void TickRenderer::Select(CPoint pt)
{
	if(m_Rect.PtInRect(pt))
		m_bSelected = true;
	else
		m_bSelected = false;
}

void TickRenderer::Render(CDC* pDC)
{
	CFont font, *pOldFont;
	COLORREF oldColor;

	CSize sz, sz2;

#define SPACE_BETWEEN_LINE	10
#define MARGIN	10

	oldColor = pDC->GetTextColor();

	int ncurx = SPACE_BETWEEN_LINE; 

	if(m_bSelected)
		pDC->FillSolidRect(&m_Rect,RGB(230,230,230));
	else
		pDC->FillSolidRect(&m_Rect,RGB(255,255,255));

	//	绘制底部分割线
	pDC->MoveTo(m_Rect.left, m_Rect.bottom - 1);
	pDC->LineTo(m_Rect.right, m_Rect.bottom - 1);

	font.CreatePointFont(160, CString("Tahoma"));
	pOldFont = pDC->SelectObject(&font);

	//	显示时间 
	CString strTime;
	int tmphour, tmpmin, tmpsec;

	tmphour = m_tick.time / 3600;
	tmpmin = m_tick.time % 3600 / 60;
	tmpsec = m_tick.time % 3600 % 60;

	strTime.Format(_T("%02d:%02d:%02d"), tmphour, tmpmin, tmpsec);

	sz = pDC->GetTextExtent(strTime);

	pDC->SetTextColor(RGB(0,0,255));
	pDC->TextOutW(m_Rect.left + (m_Rect.Width() - sz.cx) / 2, ncurx, strTime);

	ncurx += (sz.cy + SPACE_BETWEEN_LINE);

	//	显示价格
	CString strPrice, strVol;

	strPrice.Format(_T("%d"), m_tick.price);

	sz = pDC->GetTextExtent(strPrice);

	if(m_tick.bs == 'B')
		pDC->SetTextColor(RGB(255,0,0));
	else
		pDC->SetTextColor(RGB(0,150,0));

	pDC->TextOutW(m_Rect.left + (m_Rect.Width() - sz.cx) / 2, ncurx, strPrice);

	ncurx += (sz.cy + SPACE_BETWEEN_LINE);

	//	显示卖1
	pDC->SetTextColor(RGB(0,150,0));

	strPrice.Format(_T("%d"), m_tick.priceS1);
	strVol.Format(_T("%d"), m_tick.volS1);
	sz = pDC->GetTextExtent(strPrice);
	sz2 = pDC->GetTextExtent(strVol);
	pDC->TextOutW(m_Rect.left + MARGIN, ncurx, strPrice);
	pDC->TextOutW(m_Rect.right - MARGIN - sz2.cx, ncurx, strVol);
	ncurx += (sz.cy + SPACE_BETWEEN_LINE);

	//	显示买1
	pDC->SetTextColor(RGB(255,0,0));

	strPrice.Format(_T("%d"), m_tick.priceB1);
	strVol.Format(_T("%d"), m_tick.volB1);
	sz = pDC->GetTextExtent(strPrice);
	sz2 = pDC->GetTextExtent(strVol);
	pDC->TextOutW(m_Rect.left + MARGIN, ncurx, strPrice);
	pDC->TextOutW(m_Rect.right - MARGIN - sz2.cx, ncurx, strVol);
	ncurx += (sz.cy + SPACE_BETWEEN_LINE);

	pDC->SetTextColor(oldColor);
	pDC->SelectObject(pOldFont);

	font.DeleteObject();
}
