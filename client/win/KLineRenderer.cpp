#include "StdAfx.h"
#include "KLinePrint.h"
#include "KLineRenderer.h"
#include "KLineCollection.h"
#include "Utility.h"

#define ZOOM_STEP				20
#define LEFT_MARGIN				40
#define RIGHT_MARGIN			40

KLineRenderer::KLineRenderer(void)
{
	m_nKVolRatio = 3;						//	Kͼ��ɽ���ͼ�ĸ߶�Ϊ3:1
	m_nKSpace = 2;							//	K�߼��������

	m_bShowTimeLine = false;
	m_bShowVol = true;
	m_bShowKeyPrice = false;
	m_bShowAvg = false;
	m_bShowMA = false;
	m_bShowHighLow = false;
	m_bShowMaxMin = false;
	m_bShowCriticalTime = false;
	m_pKLines = NULL;
	m_bSelected = false;
	m_enRenderMode = enHighLowMode;
	m_nOpenIndex = -1;
	m_bShowDate = false;
	m_nDisplayKLineCount = m_nFirstDisplayedIndex = m_nSelectedIndex = 0;
	m_pTradeRecords = NULL;

	font.CreatePointFont(100, CString("Tahoma"));

	penRed.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
    penGreen.CreatePen(PS_SOLID, 1, RGB(0, 180, 0));
    penBlue.CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
    penBlack.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	penOrange.CreatePen(PS_SOLID,1, RGB(250, 128, 10));

	penRedBold.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    penGreenBold.CreatePen(PS_SOLID, 2, RGB(0, 180, 0));
    penBlueBold.CreatePen(PS_SOLID, 2, RGB(0, 0, 255));

	penGreyDotted.CreatePen(PS_DOT, 1, RGB(100, 100, 100));
	penRedDotted.CreatePen(PS_DASH, 1, RGB(255, 0, 0));
    penGreenDotted.CreatePen(PS_DASH, 1, RGB(0, 180, 0));
    penBlueDotted.CreatePen(PS_DASH, 1, RGB(0, 0, 255));

	brRed.CreateSolidBrush(RGB(255,0,0));
	brGreen.CreateSolidBrush(RGB(0,180,0));
	brBlue.CreateSolidBrush(RGB(0,0,255));

	m_bLossStopSelected = m_bProfitStopSelected = m_bTriggerSelected = FALSE;

	//	��ͼ,VOL, MACD�ı���Ϊ4:1:1
	m_vecDivRatio.push_back(4);
	m_vecDivRatio.push_back(1);
	m_vecDivRatio.push_back(1);
}

KLineRenderer::~KLineRenderer(void)
{
    penGreen.DeleteObject();
    penRed.DeleteObject();
    penBlue.DeleteObject();
	penBlack.DeleteObject();
	penOrange.DeleteObject();
	penGreyDotted.DeleteObject();
	penRedDotted.DeleteObject();
    penGreenDotted.DeleteObject();
    penBlueDotted.DeleteObject();
	brRed.DeleteObject();
	brGreen.DeleteObject();
	brBlue.DeleteObject();
	font.DeleteObject();
}

int KLineRenderer::GetCurTime()
{
	if(!m_pKLines || !m_pKLines->size()) return 0;

	AdjustIndex();

	if(m_nSelectedIndex < 0 || m_nSelectedIndex > (int)(m_pKLines->size()) - 1) return 0;

	return (*m_pKLines)[m_nSelectedIndex].time;
}

void KLineRenderer::AdjustIndex()
{
	if(!m_pKLines) return;

	if(m_nSelectedIndex > m_pKLines->size() - 1)
	{
		//	���K������С�ڿ���ʾ�����������ͷ��ʼ��ʾ��ѡ�����K��
		if(m_pKLines->size() <= m_nDisplayKLineCount)
		{
			m_nFirstDisplayedIndex = 0;
			m_nSelectedIndex = m_pKLines->size() - 1;
		}
		//	���K�����ݴ��ڿ���ʾ���������������K���������Ҷ˶��� 
		else
		{
			m_nFirstDisplayedIndex = m_pKLines->size() - m_nDisplayKLineCount;
			m_nSelectedIndex = m_pKLines->size() - 1;
		}		
	}

	//	���ѡ�е�K�߲��ڿ��Ƿ�Χ��
	if((m_nSelectedIndex < m_nFirstDisplayedIndex)
	  || (m_nSelectedIndex > m_nFirstDisplayedIndex + m_nDisplayKLineCount))
	{
		if(m_nSelectedIndex - m_nDisplayKLineCount / 2 >= 0)
			m_nFirstDisplayedIndex = m_nSelectedIndex - m_nDisplayKLineCount / 2;
		else
			m_nFirstDisplayedIndex = 0;
	}
}

void KLineRenderer::SelectLastK()
{
	m_nSelectedIndex = m_pKLines->size() - 1;
	AdjustIndex();
}

void KLineRenderer::SelectByTime(int nTime)
{
	if(!m_pKLines || !m_pKLines->size()) return;

	for(int i = 0; i < m_pKLines->size(); i++)
	{
		if((*m_pKLines)[i].time >= nTime) 
		{
			m_nSelectedIndex = i;
			break;
		}
	}

	AdjustIndex();

	m_nSelectedPrice = (*m_pKLines)[m_nSelectedIndex].close;
}

void KLineRenderer::Select(CPoint pt)
{
	if(!m_pKLines) return;

	if(m_Rect.PtInRect(pt))
		m_bSelected = true;
	else
		m_bSelected = false;

	if(m_bSelected)
	{
		//	ѡ��ֹ��ֹӯ��
		if(EXCHANGE.m_nPosition.nLossStop)
		{
			int nLossStopPos = GetPricePosition(EXCHANGE.m_nPosition.nLossStop);

			if( pt.y < nLossStopPos + 5 && pt.y > nLossStopPos - 5
				&& pt.x > m_Rect.left + LEFT_MARGIN
				&& pt.x < m_Rect.right - RIGHT_MARGIN)

			{
				m_bLossStopSelected = true;
				m_bProfitStopSelected = false;
				m_bTriggerSelected = false;

			}
			else
			{
				m_bLossStopSelected = false;
			}
		}

		if(EXCHANGE.m_nPosition.nProfitStop)
		{
			int nProfitStopPos = GetPricePosition(EXCHANGE.m_nPosition.nProfitStop);

			if( pt.y < nProfitStopPos + 5 && pt.y > nProfitStopPos - 5
				&& pt.x > m_Rect.left + LEFT_MARGIN
				&& pt.x < m_Rect.right - RIGHT_MARGIN)

			{
				m_bProfitStopSelected = true;
				m_bLossStopSelected = false;
				m_bTriggerSelected = false;
			}
			else
			{
				m_bProfitStopSelected = false;
			}
		}

		if(EXCHANGE.m_nPosition.nTrigger)
		{
			int nTriggerPos = GetPricePosition(EXCHANGE.m_nPosition.nTrigger);

			if( pt.y < nTriggerPos + 5 && pt.y > nTriggerPos - 5
				&& pt.x > m_Rect.left + LEFT_MARGIN
				&& pt.x < m_Rect.right - RIGHT_MARGIN)

			{
				m_bLossStopSelected = false;
				m_bProfitStopSelected = false;
				m_bTriggerSelected = true;

			}
			else
			{
				m_bTriggerSelected = false;
			}
		}

		//	����K�ߵĿ��
		float m_kWidth = (m_Rect.Width() - LEFT_MARGIN - RIGHT_MARGIN - (m_nDisplayKLineCount + 1) * m_nKSpace)
			/(float)m_nDisplayKLineCount;

		m_nSelectedIndex = (pt.x - (m_Rect.left + LEFT_MARGIN)) / (m_kWidth + m_nKSpace) + m_nFirstDisplayedIndex;

		AdjustIndex();

		if(m_nSelectedIndex < 0 || m_nSelectedIndex > (int)(m_pKLines->size()) - 1) return;

		m_nSelectedPrice = (*m_pKLines)[m_nSelectedIndex].close;
	}
}

void KLineRenderer::SetKLineData(KLineCollection* pKLines, int nDispKLineCount)
{ 
	m_pKLines = pKLines; 

	m_nDisplayKLineCount = nDispKLineCount;

	//	���K������С�ڿ���ʾ�����������ͷ��ʼ��ʾ��ѡ�����K��
	if(pKLines->size() <= m_nDisplayKLineCount)
	{
		m_nFirstDisplayedIndex = 0;
		m_nSelectedIndex = pKLines->size() - 1;
	}
	//	���K�����ݴ��ڿ���ʾ���������������K���������Ҷ˶��� 
	else
	{
		m_nFirstDisplayedIndex = pKLines->size() - m_nDisplayKLineCount;
		m_nSelectedIndex = pKLines->size() - 1;
	}
}

void KLineRenderer::ZoomIn()
{
	if(!m_pKLines || !m_bSelected) return;

	if(m_nDisplayKLineCount - ZOOM_STEP > 0)
	{
		float off = (m_nSelectedIndex - m_nFirstDisplayedIndex) / (float)m_nDisplayKLineCount;

		m_nDisplayKLineCount -= ZOOM_STEP;

		// TODO : ��֤ѡ�е�K��λ�û������䣿
		m_nFirstDisplayedIndex = m_nFirstDisplayedIndex + ZOOM_STEP * off;

		AdjustIndex();
	}
}

void KLineRenderer::ZoomOut()
{
	if(!m_pKLines || !m_bSelected) return;

	float off = (m_nSelectedIndex - m_nFirstDisplayedIndex) / (float)m_nDisplayKLineCount;

	m_nDisplayKLineCount += ZOOM_STEP;

	// TODO : ��֤ѡ�е�K��λ�û������䣿
	if(m_nFirstDisplayedIndex - ZOOM_STEP * off >= 0)
		m_nFirstDisplayedIndex = m_nFirstDisplayedIndex - ZOOM_STEP * off;
	else
		m_nFirstDisplayedIndex = 0;

	AdjustIndex();

}

void KLineRenderer::MovePrev()
{
	if(!m_pKLines || !m_bSelected) return;

	if(m_nSelectedIndex > 0)
		m_nSelectedIndex--;

	if(m_nSelectedIndex < m_nFirstDisplayedIndex)
		m_nFirstDisplayedIndex = m_nSelectedIndex;

	m_nSelectedPrice = (*m_pKLines)[m_nSelectedIndex].close;
}

void KLineRenderer::MoveNext()
{
	if(!m_pKLines || !m_bSelected) return;

	if(m_nSelectedIndex < m_pKLines->size() - 1)
		m_nSelectedIndex++;

	if(m_nSelectedIndex > m_nFirstDisplayedIndex + m_nDisplayKLineCount)
		m_nFirstDisplayedIndex = m_nSelectedIndex - m_nDisplayKLineCount;

	m_nSelectedPrice = (*m_pKLines)[m_nSelectedIndex].close;
}

void KLineRenderer::ToggleRenderMode()
{
	if(!m_bSelected) return;

	if(m_enRenderMode == enHighLowMode)
	{
		m_enRenderMode = enAxisMode;
	}
	else if(m_enRenderMode == enAxisMode)
	{
		m_enRenderMode = enHighLowMode;
	}
}

float KLineRenderer::GetMacdPosition(float fMacd)
{
	return m_vecDivStartY[2] + (m_hiMacd - fMacd) * m_pixelPerMacd;
}

float KLineRenderer::GetPricePosition(int nPrice)
{
	return m_vecDivStartY[0] + (m_kHighPrice - nPrice) * m_pixelPerPrice;
}

float KLineRenderer::GetVolPosition(int nVol)
{
	return m_vecDivStartY[2] - nVol * m_pixelPerVol;
}

float KLineRenderer::GetInterestPosition(int nInterest)
{
	return m_vecDivStartY[2] - (nInterest + m_interestMax) * m_pixelPerInterest;
}

void KLineRenderer::DelTrigger()
{
	if(m_bProfitStopSelected)
	{
		EXCHANGE.m_nPosition.nProfitStop = 0;
		m_bProfitStopSelected = false;
	}
	else if(m_bLossStopSelected)
	{
		EXCHANGE.m_nPosition.nLossStop = 0;
		m_bLossStopSelected = false;
	}
	else
	{
		EXCHANGE.m_nPosition.nTrigger = EXCHANGE.m_nPosition.nLossStop
			= EXCHANGE.m_nPosition.nProfitStop = 0;

		m_bTriggerSelected = m_bProfitStopSelected = m_bLossStopSelected = false;
	}
}

void KLineRenderer::RenderPosition(CDC* pDC)
{
	//	���Ƴɱ���
	float prPos = GetPricePosition(EXCHANGE.m_nPosition.nPrice);

	pDC->SelectObject(&penBlue);

	pDC->MoveTo(m_kMiddle + m_kWidth * 2, prPos);
	pDC->LineTo(m_Rect.right - RIGHT_MARGIN, prPos);	

	CString tmp;
	CSize sz;

	tmp.Format(_T("�ɱ�:%d(%d��) ӯ��%d"), 
				EXCHANGE.m_nPosition.nPrice, 
				EXCHANGE.m_nPosition.nSlot,
				EXCHANGE.m_nPosition.nProfit);

	sz = pDC->GetTextExtent(tmp);
	pDC->TextOutW(m_kMiddle + RIGHT_MARGIN, prPos + 1, tmp);

	//	����ֹ���
	if(EXCHANGE.m_nPosition.nLossStop)
	{
		float stopLossPos = GetPricePosition(EXCHANGE.m_nPosition.nLossStop);

		if(m_bLossStopSelected)
			pDC->SelectObject(&penGreenBold);
		else
			pDC->SelectObject(&penGreen);

		pDC->MoveTo(m_Rect.left + LEFT_MARGIN, stopLossPos);
		pDC->LineTo(m_Rect.right - RIGHT_MARGIN, stopLossPos);	

		CString tmp;
		CSize sz;

		tmp.Format(_T("ֹ��:%d"), EXCHANGE.m_nPosition.nLossStop);

		sz = pDC->GetTextExtent(tmp);
		pDC->TextOutW(m_kMiddle + RIGHT_MARGIN, stopLossPos + 1, tmp);
	}

	//	����ֹӯ��
	if(EXCHANGE.m_nPosition.nProfitStop)
	{
		float stopProfitPos = GetPricePosition(EXCHANGE.m_nPosition.nProfitStop);

		if(m_bProfitStopSelected)
			pDC->SelectObject(&penRedBold);
		else
			pDC->SelectObject(&penRed);

		pDC->MoveTo(m_Rect.left + LEFT_MARGIN, stopProfitPos);
		pDC->LineTo(m_Rect.right - RIGHT_MARGIN, stopProfitPos);	

		CString tmp;
		CSize sz;

		tmp.Format(_T("ֹӯ:%d"), EXCHANGE.m_nPosition.nProfitStop);

		sz = pDC->GetTextExtent(tmp);
		pDC->TextOutW(m_kMiddle + RIGHT_MARGIN, stopProfitPos + 1, tmp);
	}

	//	���ƴ�����
	if(EXCHANGE.m_nPosition.nTrigger)
	{
		float triggerPos = GetPricePosition(EXCHANGE.m_nPosition.nTrigger);

		if(m_bTriggerSelected)
			pDC->SelectObject(&penBlueBold);
		else
			pDC->SelectObject(&penBlue);

		pDC->MoveTo(m_Rect.left + LEFT_MARGIN, triggerPos);
		pDC->LineTo(m_Rect.right - RIGHT_MARGIN, triggerPos);	

		CString tmp;
		CSize sz;

		tmp.Format(_T("����:%d"), EXCHANGE.m_nPosition.nTrigger);

		sz = pDC->GetTextExtent(tmp);
		pDC->TextOutW(m_kMiddle + RIGHT_MARGIN, triggerPos + 1, tmp);
	}
}

void KLineRenderer::RenderKeyPrice(CDC* pDC)
{
	//	���ƹؼ��۸���(�ı���Ҫ�������⻥�า��)
	map<int, string>::iterator itKeyPrice;
	map<int, string>& keyPrices = m_pKLines->GetKeyPrices();

	pDC->SelectObject(&penGreenDotted);

	for(itKeyPrice = keyPrices.begin(); itKeyPrice != keyPrices.end(); itKeyPrice++)
	{
		if(itKeyPrice->first > m_kHighPrice || itKeyPrice->first < m_kLowPrice) 
			continue;

		float keyPricePos = GetPricePosition(itKeyPrice->first);

		pDC->MoveTo(m_Rect.left + LEFT_MARGIN, keyPricePos);
		pDC->LineTo(m_Rect.right - RIGHT_MARGIN, keyPricePos);	
		pDC->TextOutW(m_Rect.left + 5, keyPricePos - 5, CString(itKeyPrice->second.c_str()));
	}
}

void KLineRenderer::RenderMaxMinPrice(CDC* pDC)
{
	float maxPricePos = GetPricePosition(m_pKLines->m_nMaxPrice);
	float minPricePos = GetPricePosition(m_pKLines->m_nMinPrice);

	pDC->SelectObject(&penRed);
	pDC->MoveTo(m_Rect.left + LEFT_MARGIN, maxPricePos);
	pDC->LineTo(m_kMiddle, maxPricePos);	

	CString tmp;
	CSize sz;

	tmp.Format(_T("%d"), m_pKLines->m_nMaxPrice);

	sz = pDC->GetTextExtent(tmp);
	pDC->TextOutW(m_kMiddle - sz.cx, maxPricePos - sz.cy, tmp);

	pDC->SelectObject(&penGreen);
	pDC->MoveTo(m_Rect.left + LEFT_MARGIN, minPricePos);
	pDC->LineTo(m_kMiddle, minPricePos);	

	tmp.Format(_T("%d"), m_pKLines->m_nMinPrice);

	sz = pDC->GetTextExtent(tmp);
	pDC->TextOutW(m_kMiddle - sz.cx, minPricePos + 1, tmp);
}

void KLineRenderer::RenderAxis(CDC* pDC)
{
	CString strPercent;

	pDC->SelectObject(&penBlack);

	float axleLinePos = GetPricePosition(m_kAxisPrice);
	pDC->MoveTo(m_Rect.left + LEFT_MARGIN, axleLinePos);
	pDC->LineTo(m_Rect.right - RIGHT_MARGIN, axleLinePos);

	//	��ʾ���շ���
	float todayOpen = (*m_pKLines)[m_nOpenIndex].open;
	float prevClose = (*m_pKLines)[0].close;
	float gap = (todayOpen - prevClose) / prevClose * 100;
	int range = m_pKLines->m_nMaxPrice - m_pKLines->m_nMinPrice;

	KLine tmpk = (*m_pKLines)[m_nSelectedIndex];

	strPercent.Format(_T("ͼ%.2f%% ��%.2f%% ����%d�� ��ƫ%d��"), 
				(m_fPricePercentage / 0.01), gap, range, tmpk.avg_devi);

	pDC->TextOutW(m_Rect.left + LEFT_MARGIN + 10, m_Rect.top + 20, strPercent);
}

void KLineRenderer::RenderKLine(CDC* pDC, int nKIdx)
{
	KLine& kline = (*m_pKLines)[nKIdx];

	//	���Ǻ죬�µ���
	if(kline.open > kline.close)
		pDC->SelectObject(&penGreen);
	else if(kline.open < kline.close)
		pDC->SelectObject(&penRed);
	else
		pDC->SelectObject(&penBlue);

	//	ʵ����
	pDC->MoveTo(m_kLeft, m_kOpenPos);
	pDC->LineTo(m_kLeft, m_kClosePos);
	pDC->LineTo(m_kRight, m_kClosePos);
	pDC->LineTo(m_kRight, m_kOpenPos);
	pDC->LineTo(m_kLeft, m_kOpenPos);

	//	Ӱ��
	if(kline.open > kline.close)
	{
		pDC->MoveTo(m_kMiddle, m_kHighPos);
		pDC->LineTo(m_kMiddle, m_kOpenPos);
		pDC->MoveTo(m_kMiddle, m_kClosePos);
		pDC->LineTo(m_kMiddle, m_kLowPos);
	}
	else
	{
		pDC->MoveTo(m_kMiddle, m_kHighPos);
		pDC->LineTo(m_kMiddle, m_kClosePos);
		pDC->MoveTo(m_kMiddle, m_kOpenPos);
		pDC->LineTo(m_kMiddle, m_kLowPos);
	}
}

void KLineRenderer::RenderTradeRecord(CDC* pDC, int nKIdx)
{
	KLine& kline = (*m_pKLines)[nKIdx];

	for(int nTrIdx = 0; nTrIdx < m_pTradeRecords->size(); nTrIdx++)
	{
		TradeRecord tr = (*m_pTradeRecords)[nTrIdx];

		//	����λ�ڸ�K����
		if(tr.nSimuTime >= kline.start_time
			&& tr.nSimuTime < kline.time)
		{
			float kPrPos = GetPricePosition(tr.nPrice);

			if(nTrIdx == m_pTradeRecords->GetSelIdx())
			{
				pDC->SelectObject(&penBlue);
				pDC->SelectObject(&brBlue);

				pDC->BeginPath();
				pDC->MoveTo(m_kMiddle, m_kHighPos - 10);
				pDC->LineTo(m_kLeft, m_kHighPos - 15);
				pDC->LineTo(m_kRight, m_kHighPos - 15);
				pDC->LineTo(m_kMiddle, m_kHighPos - 10);
				pDC->EndPath();

				pDC->StrokeAndFillPath();
			}

			//	���Ƽ�ͷ
			if(tr.bBuy) 
			{
				pDC->SelectObject(&penRed);
				pDC->SelectObject(&brRed);

				pDC->BeginPath();
				pDC->MoveTo(m_kMiddle, kPrPos);
				pDC->LineTo(m_kLeft, kPrPos + 5);
				pDC->LineTo(m_kRight, kPrPos + 5);
				pDC->LineTo(m_kMiddle, kPrPos);
				pDC->EndPath();
			}
			else
			{
				pDC->SelectObject(&penGreen);
				pDC->SelectObject(&brGreen);

				pDC->BeginPath();
				pDC->MoveTo(m_kMiddle, kPrPos);
				pDC->LineTo(m_kLeft, kPrPos - 5);
				pDC->LineTo(m_kRight, kPrPos - 5);
				pDC->LineTo(m_kMiddle, kPrPos);
				pDC->EndPath();
			}

			if(tr.bOpen)
				pDC->StrokeAndFillPath();
			else
				pDC->StrokePath();
		}
	}
}

void KLineRenderer::RenderCriticalTime(CDC* pDC, int nKIdx)
{
	KLine& kline = (*m_pKLines)[nKIdx];
	float timeLinePos = GetPricePosition(m_kLowPrice);

	int tmpHour = kline.time / 1000 / 3600;
	int tmpMinute =  kline.time / 1000 % 3600 / 60;
	int tmpSecond =  kline.time / 1000 % 3600 % 60;
	
	int tmpTime = tmpHour * 60 + tmpMinute;

	if(tmpTime % 30 == 0)
	{
		pDC->SelectObject(&penRedDotted);		
	}
	else if(tmpTime % 15 == 0)
	{
		pDC->SelectObject(&penBlueDotted);
	}
	else
	{
		pDC->SelectObject(&penBlueDotted);
	}

	if(tmpTime % 15 == 0)
	{		
		pDC->MoveTo(m_kMiddle, m_Rect.top);
		pDC->LineTo(m_kMiddle, m_kHighPos - 10);

		pDC->MoveTo(m_kMiddle, m_kLowPos + 10);
		pDC->LineTo(m_kMiddle, timeLinePos);

		CString tmptime;

		tmptime.Format(_T("%02d:%02d"), tmpHour, tmpMinute);

		CSize sz = pDC->GetTextExtent(tmptime);
		pDC->TextOutW(m_kMiddle, timeLinePos - sz.cy, tmptime);
	}
}

void KLineRenderer::RenderHighLowPrice(CDC* pDC, int nKIdx)
{
	KLine& kline = (*m_pKLines)[nKIdx];

	float kH20Pos = GetPricePosition(kline.high20);
	float kH60Pos = GetPricePosition(kline.high60);

	float kL20Pos = GetPricePosition(kline.low20);
	float kL60Pos = GetPricePosition(kline.low60);

	pDC->SelectObject(&penBlue);

	if(kH20Pos < m_Rect.bottom && kH20Pos > m_Rect.top)
	{
		pDC->MoveTo(m_kLeft - m_nKSpace / 2, kH20Pos);
		pDC->LineTo(m_kRight + m_nKSpace / 2, kH20Pos);
	}

	if(kL20Pos < m_Rect.bottom && kL20Pos > m_Rect.top)
	{
		pDC->MoveTo(m_kLeft - m_nKSpace / 2, kL20Pos);
		pDC->LineTo(m_kRight + m_nKSpace / 2, kL20Pos);
	}

	pDC->SelectObject(&penRed);

	if(kH60Pos < m_Rect.bottom && kH60Pos > m_Rect.top)
	{
		pDC->MoveTo(m_kLeft  - m_nKSpace / 2, kH60Pos);
		pDC->LineTo(m_kRight + m_nKSpace / 2, kH60Pos);
	}

	if(kL60Pos < m_Rect.bottom && kL60Pos > m_Rect.top)
	{
		pDC->MoveTo(m_kLeft  - m_nKSpace / 2, kL60Pos);
		pDC->LineTo(m_kRight + m_nKSpace / 2, kL60Pos);
	}
}

void KLineRenderer::RenderSelection(CDC* pDC, int nKIdx)
{
	KLine& kline = (*m_pKLines)[nKIdx];

	pDC->SelectObject(&penGreyDotted);

	if(!m_bSelected)	//	δѡ��
	{
		float kCurPos = GetPricePosition(m_nSelectedPrice);

		pDC->MoveTo(m_Rect.left + LEFT_MARGIN, kCurPos);
		pDC->LineTo(m_kMiddle - m_kWidth * 2, kCurPos);

		pDC->MoveTo(m_kMiddle + m_kWidth * 2, kCurPos);
		pDC->LineTo(m_Rect.right, kCurPos);

		pDC->MoveTo(m_kMiddle, m_Rect.top);
		pDC->LineTo(m_kMiddle, m_kHighPos - 10);

		pDC->MoveTo(m_kMiddle, m_kLowPos + 10);
		pDC->LineTo(m_kMiddle, m_Rect.bottom);
	}
	else	//	ѡ��
	{
		float kCurPos = GetPricePosition(kline.close);

		pDC->MoveTo(m_Rect.left + LEFT_MARGIN, kCurPos);
		pDC->LineTo(m_kMiddle - m_kWidth * 2, kCurPos);

		pDC->MoveTo(m_kMiddle, m_Rect.top);
		pDC->LineTo(m_kMiddle, m_kHighPos - 10);

		pDC->MoveTo(m_kMiddle, m_kLowPos + 10);
		pDC->LineTo(m_kMiddle, m_Rect.bottom);

		CString tmp;

		tmp.Format(_T("%d"), kline.close);
		pDC->TextOutW(m_kMiddle + m_kWidth * 2, kCurPos, tmp);

		//	���Ƴֲ�����
		float kInterestPos = GetInterestPosition(kline.interest);

		pDC->MoveTo(m_Rect.left + LEFT_MARGIN, kInterestPos);
		pDC->LineTo(m_kMiddle - m_kWidth * 2, kInterestPos);

		tmp.Format(_T("%d"), kline.interest);
		pDC->TextOutW(m_kMiddle + m_kWidth * 2, kInterestPos, tmp);

		//	���Ƴɽ�����
//		float kVolPos = GetVolPosition(kline.vol);

//		pDC->MoveTo(m_Rect.left + LEFT_MARGIN, kVolPos);
//		pDC->LineTo(m_kMiddle - m_kWidth * 2, kVolPos);

//		tmp.Format(_T("%d"), kline.vol);
//		CSize sz = pDC->GetTextExtent(tmp);
//		pDC->TextOutW(m_kMiddle + m_kWidth * 2, kVolPos - sz.cy, tmp);

		//  ����MACD��
		float kMacdPos = GetMacdPosition(kline.MACD);

		pDC->MoveTo(m_Rect.left + LEFT_MARGIN, kMacdPos);
		pDC->LineTo(m_kMiddle - m_kWidth * 2, kMacdPos);
		tmp.Format(_T("%.2f"), kline.MACD);
		pDC->TextOutW(m_kMiddle + m_kWidth * 2, kMacdPos, tmp);
	}

	// ��ʾʱ��ͼ۸�

	CString strTime, strOpen, strCur;

	if(kline.open >= kline.close) /* �� */
	{
		strCur.Format(_T("-%d -%.2f%%"), 
					kline.open - kline.close, 
					100 * (kline.open - kline.close) / (float)kline.open);
	}
	else
	{
		strCur.Format(_T("+%d +%.2f%%"), 
					kline.close - kline.open, 
					100 * (kline.close - kline.open) / (float)kline.open);	
	}

	if(m_nOpenIndex >= 0)
	{
		KLine kopen = (*m_pKLines)[m_nOpenIndex];
		
		if(kline.close > kopen.open)
		{
			strOpen.Format(_T("+%d +%.2f%%"), 
						kline.close - kopen.open, 
						100 * (kline.close - kopen.open) / (float)kopen.open);	

		}
		else
		{
			strOpen.Format(_T("-%d -%.2f%%"), 
						kopen.open - kline.close, 
						100 * (kopen.open - kline.close) / (float)kopen.open);
		}
	}

	if(m_bShowDate)/* ��ʾ���ڼ� */
	{
		int weekday = Utility::GetWeekDayByDate(kline.time);
		strTime.Format(_T("%d - ����%d - %s"), kline.time, weekday, strCur);				
	}
	else
	{
		strTime.Format(_T("%d %s"), 
			Utility::ConvContTimeToDispTime(kline.time / 1000), strOpen);
	}

	CSize sz = pDC->GetTextExtent(strTime);

	if(m_kMiddle + 1 + sz.cx > m_Rect.right)
	{
		pDC->TextOutW(m_kMiddle - sz.cx - 5, m_Rect.top + 1, strTime);
	}
	else
	{
		pDC->TextOutW(m_kMiddle + 5, m_Rect.top + 1, strTime);
	}
}

void KLineRenderer::RenderAvg(CDC* pDC, int nKIdx)
{
	if(nKIdx > m_nFirstDisplayedIndex + 1) 
	{
		KLine& prev_kline = (*m_pKLines)[nKIdx - 1];
		KLine& kline = (*m_pKLines)[nKIdx];

		float kLastAvgPos = GetPricePosition(prev_kline.avg);
		float kAvgPos = GetPricePosition(kline.avg);

		float kLastMiddle = m_kMiddle - m_kWidth - m_nKSpace;

		pDC->SelectObject(&penOrange);
		pDC->MoveTo(kLastMiddle, kLastAvgPos);
		pDC->LineTo(m_kMiddle, kAvgPos);
	}
}

void KLineRenderer::RenderMACD(CDC* pDC, int nKIdx)
{
	if(nKIdx > m_nFirstDisplayedIndex + 1)
	{
		KLine& prev_kline = (*m_pKLines)[nKIdx - 1];
		KLine& kline = (*m_pKLines)[nKIdx];

		float kMacdPos = GetMacdPosition(kline.MACD);
		float kMacdAvgPos = GetMacdPosition(kline.avgMACD9);

		float kLastMacdPos = GetMacdPosition(prev_kline.MACD);
		float kLastMacdAvgPos = GetMacdPosition(prev_kline.avgMACD9);

		float kLastMiddle = m_kMiddle - m_kWidth - m_nKSpace;

		pDC->SelectObject(&penOrange);
		pDC->MoveTo(kLastMiddle, kLastMacdPos);
		pDC->LineTo(m_kMiddle, kMacdPos);

		pDC->SelectObject(&penGreen);
		pDC->MoveTo(kLastMiddle, kLastMacdAvgPos);
		pDC->LineTo(m_kMiddle, kMacdAvgPos);
	}
}

void KLineRenderer::RenderMA(CDC* pDC, int nKIdx)
{
	if(nKIdx > m_nFirstDisplayedIndex + 1)
	{
		KLine& prev_kline = (*m_pKLines)[nKIdx - 1];
		KLine& kline = (*m_pKLines)[nKIdx];

		float kMA20Pos = GetPricePosition(kline.ma20);
		float kMA60Pos = GetPricePosition(kline.ma60);

		float kLastMA20Pos = GetPricePosition(prev_kline.ma20);
		float kLastMA60Pos = GetPricePosition(prev_kline.ma60);

		float kLastMiddle = m_kMiddle - m_kWidth - m_nKSpace;

		pDC->SelectObject(&penBlack);

		if(kLastMA20Pos > m_Rect.top && kLastMA20Pos < m_Rect.bottom)
		{
			pDC->MoveTo(kLastMiddle, kLastMA20Pos);
			pDC->LineTo(m_kMiddle, kMA20Pos);
		}

		if(kLastMA60Pos > m_Rect.top && kLastMA60Pos < m_Rect.bottom)
		{
			pDC->MoveTo(kLastMiddle, kLastMA60Pos);
			pDC->LineTo(m_kMiddle, kMA60Pos);
		}
	}
}

void KLineRenderer::RenderVol(CDC* pDC, int nKIdx)
{
	int divBottom = m_vecDivStartY[2];

	KLine& kline = (*m_pKLines)[nKIdx];

	float kLastMiddle = m_kMiddle - m_kWidth - m_nKSpace;

	//	�ɽ�����
	float kVolPos = GetVolPosition(kline.vol); 

	if(m_bShowTimeLine)
	{
		pDC->SelectObject(penBlue);
		pDC->MoveTo(m_kMiddle - 2, divBottom);
		pDC->LineTo(m_kMiddle - 2, kVolPos);
	}
	else
	{
		if(kline.open > kline.close)
			pDC->SelectObject(&penGreen);
		else if(kline.open < kline.close)
			pDC->SelectObject(&penRed);
		else
			pDC->SelectObject(&penBlue);

		pDC->MoveTo(m_kLeft, divBottom);
		pDC->LineTo(m_kLeft, kVolPos);
		pDC->LineTo(m_kRight, kVolPos);
		pDC->LineTo(m_kRight, divBottom);
	}

	//	���ڳֲ�����
	int nInterest, nPrevInterest;

	nPrevInterest = (nKIdx == 0) ? 0 : (*m_pKLines)[nKIdx - 1].interest;
	nInterest = kline.interest;

	float kPrevInterestPos = GetInterestPosition(nPrevInterest);
	float kInterestPos = GetInterestPosition(nInterest);

	pDC->SelectObject(&penBlue);
	pDC->MoveTo(kLastMiddle, kPrevInterestPos);
	pDC->LineTo(m_kMiddle, kInterestPos);
}

void KLineRenderer::RenderGraphFrame(CDC* pDC)
{
	float timeLinePos = GetPricePosition(m_kLowPrice);

	pDC->SelectObject(&penBlack);

	//	���Ʒָ���
	pDC->MoveTo(m_Rect.left + LEFT_MARGIN, timeLinePos);
	pDC->LineTo(m_Rect.right - RIGHT_MARGIN, timeLinePos);

	CString tmp;
	CSize sz;

	//	��ʾ���ɽ���
	tmp.Format(_T("%d"), m_volMax);
	sz = pDC->GetTextExtent(tmp);
	pDC->TextOutW(m_Rect.left + LEFT_MARGIN - sz.cx, timeLinePos, tmp);

	//	��ʾ���ֲ�
	tmp.Format(_T("%d"), m_interestMax);
	sz = pDC->GetTextExtent(tmp);
	pDC->TextOutW(m_Rect.right - RIGHT_MARGIN, timeLinePos, tmp);

	//	������ͼ������ͼ�ķָ���
	for(int i = 0; i < m_vecDivStartY.size(); i++)
	{
		pDC->MoveTo(m_Rect.left, m_vecDivStartY[i]);
		pDC->LineTo(m_Rect.right, m_vecDivStartY[i]);		
	}

	//	����ʱ����
	pDC->MoveTo(m_Rect.left, m_Rect.bottom - 1);
	pDC->LineTo(m_Rect.right, m_Rect.bottom - 1);

	//	���Ƽ۸���
	pDC->MoveTo(m_Rect.left + 1, m_Rect.top);
	pDC->LineTo(m_Rect.left + 1, m_Rect.bottom);

	pDC->MoveTo(m_Rect.left + LEFT_MARGIN, m_Rect.top);
	pDC->LineTo(m_Rect.left + LEFT_MARGIN, m_Rect.bottom);

	pDC->MoveTo(m_Rect.right - RIGHT_MARGIN, m_Rect.top);
	pDC->LineTo(m_Rect.right - RIGHT_MARGIN, m_Rect.bottom);

	// ����MACD����

	float macd0Pos = GetMacdPosition(0);

	pDC->MoveTo(m_Rect.left + LEFT_MARGIN, macd0Pos);
	pDC->LineTo(m_Rect.right - RIGHT_MARGIN, macd0Pos);
}

void KLineRenderer::CalculateGraphLayout()
{
	//	���������ͼ��λ��
	int nTotalRatio = 0;

	for(int i = 0; i < m_vecDivRatio.size(); i++) 
		nTotalRatio += m_vecDivRatio[i];

	m_vecDivHeight.clear();
	m_vecDivStartY.clear();

	int nStartYPosition = m_Rect.top;

	for(int i = 0; i < m_vecDivRatio.size(); i++)
	{
		int nDivHeight = (float)m_Rect.Height() * m_vecDivRatio[i] / nTotalRatio;
		m_vecDivHeight.push_back(nDivHeight);
		m_vecDivStartY.push_back(nStartYPosition);
		nStartYPosition += nDivHeight;
	}

	if(m_enRenderMode == enHighLowMode)
	{
		m_pKLines->GetPriceVolRange(m_nFirstDisplayedIndex, 
									m_nFirstDisplayedIndex + m_nDisplayKLineCount, 
									m_kHighPrice, m_kLowPrice, 
									m_volMax, m_interestMax,
									m_hiMacd, m_loMacd);

		//	�ߵ͵��Ĳ�������
		m_fPricePercentage = (m_kHighPrice - m_kLowPrice) /((m_kHighPrice + m_kLowPrice) / 2.0f);
	}
	else if(m_enRenderMode == enAxisMode)
	{
		if(m_nOpenIndex > m_pKLines->size() - 1) return;

		m_pKLines->GetPriceVolRange(1, m_pKLines->size() - 1, 
								m_kHighPrice, m_kLowPrice, 
								m_volMax, m_interestMax,
								m_hiMacd, m_loMacd);

		//	��������ʾ����
		m_hiMacd += 2.0;
		m_loMacd -= 2.0;

		//	ȷ����ʾMACD 0��
		if(m_hiMacd < 2.0) m_hiMacd = 2.0;
		if(m_loMacd > -2.0) m_loMacd = -2.0;

		// �Կ���Ϊ�����
		m_kAxisPrice = (*m_pKLines)[m_nOpenIndex].open;
		int min = m_kLowPrice;
		int max = m_kHighPrice;

		if(max - m_kAxisPrice > m_kAxisPrice - min)
		{
			m_kHighPrice = m_kAxisPrice + (max - m_kAxisPrice);
			m_kLowPrice = m_kAxisPrice - (max - m_kAxisPrice);
		}
		else
		{
			m_kHighPrice = m_kAxisPrice + (m_kAxisPrice - min);
			m_kLowPrice = m_kAxisPrice - (m_kAxisPrice - min);
		}

		m_fPricePercentage = (m_kHighPrice - m_kAxisPrice)/(float)m_kAxisPrice;

		float fDefaultRange = 0.02f;

		if( m_fPricePercentage < fDefaultRange)
		{
			//	������С��ͳһʹ�� 2% ���ǵ�������
			m_kHighPrice = m_kAxisPrice * (1 + fDefaultRange);
			m_kLowPrice = m_kAxisPrice * (1 - fDefaultRange);
			m_fPricePercentage = fDefaultRange;
		}
		else // �����ȵ����Դ��Ա㿴�������ľ��� 
		{
			m_fPricePercentage += 0.003f;
			m_kHighPrice = m_kAxisPrice * ( 1 + m_fPricePercentage);
			m_kLowPrice = m_kAxisPrice * ( 1 - m_fPricePercentage);
		}
	}

	if(m_bShowVol)
	{
		//	����һ���۸��Ӧ��������
		m_pixelPerPrice = m_vecDivHeight[0] / (m_kHighPrice - m_kLowPrice);
		
		//	���㵥λ�ɽ�����Ӧ��������
		m_pixelPerVol = m_vecDivHeight[1] / m_volMax;

		m_pixelPerInterest = m_vecDivHeight[1] / (2 * m_interestMax);

		m_pixelPerMacd = m_vecDivHeight[2] / (m_hiMacd - m_loMacd);
	}
	else
	{
		m_pixelPerPrice = (float)m_Rect.Height() / (m_kHighPrice - m_kLowPrice);
		m_pixelPerVol = 0;
	}

	//	����K�ߵĿ��
	m_kWidth = (m_Rect.Width() - LEFT_MARGIN - RIGHT_MARGIN - (m_nDisplayKLineCount + 1) * m_nKSpace)
		/(float)m_nDisplayKLineCount;
}

void KLineRenderer::CalculateKLineLayout(int nKIdx)
{
	/* �������ҷ��� */
	m_kLeft = m_Rect.left + LEFT_MARGIN + (nKIdx - m_nFirstDisplayedIndex + 1) * m_nKSpace
							+ m_kWidth * (nKIdx - m_nFirstDisplayedIndex);

	m_kMiddle = m_kLeft + m_kWidth / 2;
	m_kRight = m_kLeft + m_kWidth;

	KLine& kline = (*m_pKLines)[nKIdx];

	m_kHighPos = GetPricePosition(kline.high);
	m_kLowPos = GetPricePosition(kline.low);
	m_kOpenPos = GetPricePosition(kline.open);
	m_kClosePos = GetPricePosition(kline.close);
}

void KLineRenderer::RenderTimeLine(CDC* pDC, int nKIdx)
{
	if(nKIdx <= 1) return;

	KLine& prev_kline = (*m_pKLines)[nKIdx-1];
	KLine& kline = (*m_pKLines)[nKIdx];

	float kLastMiddle = m_kMiddle - m_kWidth - m_nKSpace;
	float kLastClose = GetPricePosition(prev_kline.close);

	pDC->SelectObject(&penBlue);

	pDC->MoveTo(kLastMiddle, kLastClose);
	pDC->LineTo(m_kMiddle, m_kClosePos);
}

void KLineRenderer::Render(CDC* pDC)
{
	if(!m_pKLines || !m_pKLines->size() || (m_Rect == CRect(0,0,0,0))) 
		return;

	pDC->SelectObject(&font);

	if(m_bSelected)
		pDC->FillSolidRect(&m_Rect,RGB(230,230,230));
	else
		pDC->FillSolidRect(&m_Rect,RGB(255,255,255));

	CalculateGraphLayout();

	RenderGraphFrame(pDC);

	for(int i = m_nFirstDisplayedIndex; i <= m_nFirstDisplayedIndex + m_nDisplayKLineCount; i++)
	{
		if( i >= m_pKLines->size()) break;

		CalculateKLineLayout(i);
		
		//	����20/60�ոߵ͵�
		if(m_bShowHighLow)
			RenderHighLowPrice(pDC, i);

		/* ���ƾ����� */
		if(m_bShowAvg)
			RenderAvg(pDC, i);

		/* ����MA20/60 */
		if(m_bShowMA)
			RenderMA(pDC, i);

		RenderMACD(pDC, i);

		/* ����15���ӣ�30���ӣ�60����ʱ���� */
		if(m_bShowCriticalTime)
			RenderCriticalTime(pDC, i);

		if(m_bShowTimeLine)		//	��ʾ��ʱ��
		{
			RenderTimeLine(pDC,i);
		}
		else
		{
			//	����K��
			RenderKLine(pDC, i);
		}

		//	���ƽ��׼�¼
		if(m_pTradeRecords)
			RenderTradeRecord(pDC, i);

		//	���Ƴɽ���/�ֲ�����Ϣ
		if(m_bShowVol)
			RenderVol(pDC, i);

		//	��ʾѡ����Ϣ
		if(i == m_nSelectedIndex)
			RenderSelection(pDC, i);
	}

	//	���ƹؼ��۸�
	if(m_bShowKeyPrice)
		RenderKeyPrice(pDC);

	//	���Ƶ��������͵�
	if(m_bShowMaxMin)
		RenderMaxMinPrice(pDC);

	//	����������
	if(m_enRenderMode == enAxisMode)
		RenderAxis(pDC);

	//	����ͷ����Ϣ
	RenderPosition(pDC);

	//	��ʾ�۸�ٷֱȷ�Χ
	CString strPercent;
	strPercent.Format(_T("%.2f"), (m_fPricePercentage / 0.01));
	pDC->TextOutW(m_Rect.left + 5, m_Rect.top + 1, strPercent);
}

void KLineRenderer::SetLossStopPrice(int price)
{
	TradeFacility& ex = EXCHANGE;

	if(m_bLossStopSelected)
	{
		ex.m_nPosition.nLossStop = price;
	}
	else if((!ex.m_nPosition.nLossStop)
		&& (ex.m_nPosition.nSlot || ex.m_nPosition.nTrigger)
		&& (!m_bTriggerSelected)
		&& (!m_bProfitStopSelected))
	{
		ex.m_nPosition.nLossStop = price;
		m_bLossStopSelected = true;
		m_bProfitStopSelected = false;
		m_bTriggerSelected = false;
	}
}

void KLineRenderer::SetProfitStopPrice(int price)
{
	if(m_bProfitStopSelected) 
	{
		EXCHANGE.m_nPosition.nProfitStop = price;
	}
	else if((!EXCHANGE.m_nPosition.nProfitStop) 
		&& EXCHANGE.m_nPosition.nLossStop
		&&(!m_bLossStopSelected)
		&&(!m_bTriggerSelected))
	{
		EXCHANGE.m_nPosition.nProfitStop = price;
		m_bLossStopSelected = false;
		m_bProfitStopSelected = true;
		m_bTriggerSelected = false;

	}
}

void KLineRenderer::SetTriggerPrice(int price)
{
	//	�Ѿ���ͷ���ˣ����������ô�����
	if(EXCHANGE.m_nPosition.nSlot) return;

	if(m_bTriggerSelected)
	{
		EXCHANGE.m_nPosition.nTrigger = price;
	}
	else if((!EXCHANGE.m_nPosition.nSlot) && (!EXCHANGE.m_nPosition.nTrigger))
	{
		EXCHANGE.m_nPosition.nTrigger = price;
		m_bLossStopSelected = false;
		m_bProfitStopSelected = false;
		m_bTriggerSelected = true;
	}
}

void KLineRenderer::SetSelectedPrice(int price)
{
	m_nSelectedPrice = price;
}

int KLineRenderer::GetSelectedClosePrice()
{
	if(!m_pKLines || !m_pKLines->size()) return 0;

	if(m_nSelectedIndex < 0 || m_nSelectedIndex > (int)(m_pKLines->size()) - 1) return 0;

	return (*m_pKLines)[m_nSelectedIndex].close;
}

int KLineRenderer::GetPriceByPosition(CPoint pt)
{
	if(!m_bSelected) return 0;
	if(!m_Rect.PtInRect(pt)) return 0;

	m_cp = pt;

	return (m_kHighPrice - (pt.y - m_Rect.top) / m_pixelPerPrice);
}
