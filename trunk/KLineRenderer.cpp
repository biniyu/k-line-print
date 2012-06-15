#include "StdAfx.h"
#include "KLineRenderer.h"
#include "KLineCollection.h"
#include "Utility.h"

#define ZOOM_STEP				20
#define LEFT_MARGIN				40
#define RIGHT_MARGIN			40

KLineRenderer::KLineRenderer(void)
{
	m_nKVolRatio = 3;						//	K图与成交量图的高度为3:1
	m_nKSpace = 2;							//	K线间的像素数
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
	m_enTrackingMode = enCloseTMode;
	m_nOpenIndex = -1;
	m_bShowDate = false;
	m_nDisplayKLineCount = m_nFirstDisplayedIndex = m_nSelectedIndex = 0;
	m_pTradeRecords = NULL;

	font.CreatePointFont(100, CString("Tahoma"));

	penRed.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
    penGreen.CreatePen(PS_SOLID, 1, RGB(0, 180, 0));
    penBlue.CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
    penBlack.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

	penGreyDotted.CreatePen(PS_DOT, 1, RGB(100, 100, 100));
	penRedDotted.CreatePen(PS_DASH, 1, RGB(255, 0, 0));
    penGreenDotted.CreatePen(PS_DASH, 1, RGB(0, 180, 0));
    penBlueDotted.CreatePen(PS_DASH, 1, RGB(0, 0, 255));

	brRed.CreateSolidBrush(RGB(255,0,0));
	brGreen.CreateSolidBrush(RGB(0,180,0));
}

KLineRenderer::~KLineRenderer(void)
{
    penGreen.DeleteObject();
    penRed.DeleteObject();
    penBlue.DeleteObject();
	penBlack.DeleteObject();
	penGreyDotted.DeleteObject();
	penRedDotted.DeleteObject();
    penGreenDotted.DeleteObject();
    penBlueDotted.DeleteObject();
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
		//	如果K线数据小于可显示的数量，则从头开始显示，选中最后K线
		if(m_pKLines->size() <= m_nDisplayKLineCount)
		{
			m_nFirstDisplayedIndex = 0;
			m_nSelectedIndex = m_pKLines->size() - 1;
		}
		//	如果K线数据大于可显示的数量，则从最后的K线与区域右端对齐 
		else
		{
			m_nFirstDisplayedIndex = m_pKLines->size() - m_nDisplayKLineCount;
			m_nSelectedIndex = m_pKLines->size() - 1;
		}		
	}

	//	如果选中的K线不在可是范围内
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
		//	计算K线的宽度
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

	//	如果K线数据小于可显示的数量，则从头开始显示，选中最后K线
	if(pKLines->size() <= m_nDisplayKLineCount)
	{
		m_nFirstDisplayedIndex = 0;
		m_nSelectedIndex = pKLines->size() - 1;
	}
	//	如果K线数据大于可显示的数量，则从最后的K线与区域右端对齐 
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

		// TODO : 保证选中的K线位置基本不变？
		m_nFirstDisplayedIndex = m_nFirstDisplayedIndex + ZOOM_STEP * off;

		AdjustIndex();
	}
}

void KLineRenderer::ZoomOut()
{
	if(!m_pKLines || !m_bSelected) return;

	float off = (m_nSelectedIndex - m_nFirstDisplayedIndex) / (float)m_nDisplayKLineCount;

	m_nDisplayKLineCount += ZOOM_STEP;

	// TODO : 保证选中的K线位置基本不变？
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

float KLineRenderer::GetPricePosition(int nPrice)
{
	return m_Rect.top + (m_kHighPrice - nPrice) * m_pixelPerPrice;
}

float KLineRenderer::GetVolPosition(int nVol)
{
	return m_Rect.bottom - nVol * m_pixelPerVol;
}

float KLineRenderer::GetInterestPosition(int nInterest)
{
	return m_Rect.bottom - (nInterest + m_interestMax) * m_pixelPerInterest;
}

void KLineRenderer::RenderKeyPrice(CDC* pDC)
{
	//	绘制关键价格线(文本需要错开，以免互相覆盖)
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
	pDC->LineTo(m_Rect.right - RIGHT_MARGIN, maxPricePos);	

	CString tmp;

	tmp.Format(_T("%d"), m_pKLines->m_nMaxPrice);
	pDC->TextOutW(m_Rect.right - RIGHT_MARGIN + 4, maxPricePos, tmp);

	pDC->SelectObject(&penGreen);
	pDC->MoveTo(m_Rect.left + LEFT_MARGIN, minPricePos);
	pDC->LineTo(m_Rect.right - RIGHT_MARGIN, minPricePos);	

	tmp.Format(_T("%d"), m_pKLines->m_nMinPrice);
	pDC->TextOutW(m_Rect.right - RIGHT_MARGIN + 4, minPricePos, tmp);
}

void KLineRenderer::RenderAxis(CDC* pDC)
{
	CString strPercent;

	pDC->SelectObject(&penBlack);

	float axleLinePos = GetPricePosition(m_kAxisPrice);
	pDC->MoveTo(m_Rect.left + LEFT_MARGIN, axleLinePos);
	pDC->LineTo(m_Rect.right - RIGHT_MARGIN, axleLinePos);

	//	显示跳空幅度
	float todayOpen = (*m_pKLines)[m_nOpenIndex].open;
	float prevClose = (*m_pKLines)[0].close;
	float gap = (todayOpen - prevClose) / prevClose * 100;
	int range = m_pKLines->m_nMaxPrice - m_pKLines->m_nMinPrice;

	KLine tmpk = (*m_pKLines)[m_nSelectedIndex];

	strPercent.Format(_T("图%.2f%% 跳%.2f%% 波幅%d点 均偏%d点"), 
				(m_fPricePercentage / 0.01), gap, range, tmpk.avg_devi);

	pDC->TextOutW(m_Rect.left + LEFT_MARGIN + 10, m_Rect.top + 20, strPercent);
}

void KLineRenderer::RenderKLine(CDC* pDC, int nKIdx)
{
	KLine& kline = (*m_pKLines)[nKIdx];

	//	上涨红，下跌绿
	if(kline.open > kline.close)
		pDC->SelectObject(&penGreen);
	else if(kline.open < kline.close)
		pDC->SelectObject(&penRed);
	else
		pDC->SelectObject(&penBlue);

	//	实体线
	pDC->MoveTo(m_kLeft, m_kOpenPos);
	pDC->LineTo(m_kLeft, m_kClosePos);
	pDC->LineTo(m_kRight, m_kClosePos);
	pDC->LineTo(m_kRight, m_kOpenPos);
	pDC->LineTo(m_kLeft, m_kOpenPos);

	//	影线
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

		//	正好位于该K线中
		if(tr.nSimuTime >= kline.start_time
			&& tr.nSimuTime < kline.time)
		{
			float kPrPos = GetPricePosition(tr.nPrice);

			//	绘制箭头
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

	if(!m_bSelected)	//	未选中
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
	else	//	选中
	{
		if(m_enTrackingMode == enCloseTMode)
		{
			float kCurPos = GetPricePosition(kline.close);

			pDC->MoveTo(m_Rect.left + LEFT_MARGIN, kCurPos);
			pDC->LineTo(m_kMiddle - m_kWidth * 2, kCurPos);

			pDC->MoveTo(m_kMiddle + m_kWidth * 2, kCurPos);
			pDC->LineTo(m_Rect.right - RIGHT_MARGIN, kCurPos);

			pDC->MoveTo(m_kMiddle, m_Rect.top);
			pDC->LineTo(m_kMiddle, m_kHighPos - 10);

			pDC->MoveTo(m_kMiddle, m_kLowPos + 10);
			pDC->LineTo(m_kMiddle, m_Rect.bottom);

			CString tmp;

			tmp.Format(_T("%d"), kline.close);

			pDC->TextOutW(m_Rect.right - RIGHT_MARGIN + 4, kCurPos, tmp);
		}
		else if(m_enTrackingMode == enHighLowTMode)
		{
			float kCurHighPos = GetPricePosition(kline.high);
			float kCurLowPos = GetPricePosition(kline.low);

			pDC->MoveTo(m_Rect.left + LEFT_MARGIN, kCurHighPos);
			pDC->LineTo(m_kMiddle - m_kWidth * 2, kCurHighPos);
			pDC->MoveTo(m_kMiddle + m_kWidth * 2, kCurHighPos);
			pDC->LineTo(m_Rect.right, kCurHighPos);

			pDC->MoveTo(m_Rect.left + LEFT_MARGIN, kCurLowPos);
			pDC->LineTo(m_kMiddle - m_kWidth * 2, kCurLowPos);
			pDC->MoveTo(m_kMiddle + m_kWidth * 2, kCurLowPos);
			pDC->LineTo(m_Rect.right, kCurLowPos);

			pDC->MoveTo(m_kMiddle, m_Rect.top);
			pDC->LineTo(m_kMiddle, m_kHighPos - 10);
			pDC->MoveTo(m_kMiddle, m_kLowPos + 10);
			pDC->LineTo(m_kMiddle, m_Rect.bottom);
		}
		else if(m_enTrackingMode == enMouseTMode)
		{
			//	绘制当前鼠标所在位置的跟踪线
			pDC->MoveTo(m_Rect.left + LEFT_MARGIN, m_cp.y);
			pDC->LineTo(m_Rect.right, m_cp.y);

			pDC->MoveTo(m_kMiddle, m_Rect.top);
			pDC->LineTo(m_kMiddle, m_kHighPos - 10);
			pDC->MoveTo(m_kMiddle, m_kLowPos + 10);
			pDC->LineTo(m_kMiddle, m_Rect.bottom);
		}
	}

	// 显示时间和价格

	CString strTime, strOpen, strCur;

	if(kline.open >= kline.close) /* 跌 */
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

	if(m_bShowDate)/* 显示星期几 */
	{
		int weekday = Utility::GetWeekDayByDate(kline.time);
		strTime.Format(_T("%d - 星期%d - %s"), kline.time, weekday, strCur);				
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

		pDC->SelectObject(&penBlack);
		pDC->MoveTo(kLastMiddle, kLastAvgPos);
		pDC->LineTo(m_kMiddle, kAvgPos);
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
	KLine& kline = (*m_pKLines)[nKIdx];

	float kLastMiddle = m_kMiddle - m_kWidth - m_nKSpace;

	//	成交量线
	float kVolPos = GetVolPosition(kline.vol); 

	if(kline.open > kline.close)
		pDC->SelectObject(&penGreen);
	else if(kline.open < kline.close)
		pDC->SelectObject(&penRed);
	else
		pDC->SelectObject(&penBlue);

	pDC->MoveTo(m_kLeft, m_Rect.bottom);
	pDC->LineTo(m_kLeft, kVolPos);
	pDC->LineTo(m_kRight, kVolPos);
	pDC->LineTo(m_kRight, m_Rect.bottom);

	//	日内持仓量线
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

	//	绘制分割线
	pDC->MoveTo(m_Rect.left + LEFT_MARGIN, timeLinePos);
	pDC->LineTo(m_Rect.right - RIGHT_MARGIN, timeLinePos);

	//	绘制时间线
	pDC->MoveTo(m_Rect.left, m_Rect.bottom - 1);
	pDC->LineTo(m_Rect.right, m_Rect.bottom - 1);

	//	绘制价格线
	pDC->MoveTo(m_Rect.left + 1, m_Rect.top);
	pDC->LineTo(m_Rect.left + 1, m_Rect.bottom);

	pDC->MoveTo(m_Rect.left + LEFT_MARGIN, m_Rect.top);
	pDC->LineTo(m_Rect.left + LEFT_MARGIN, m_Rect.bottom);

	pDC->MoveTo(m_Rect.right - RIGHT_MARGIN, m_Rect.top);
	pDC->LineTo(m_Rect.right - RIGHT_MARGIN, m_Rect.bottom);
}

void KLineRenderer::CalculateGraphLayout()
{
	if(m_enRenderMode == enHighLowMode)
	{
		m_pKLines->GetPriceVolRange(m_nFirstDisplayedIndex, 
									m_nFirstDisplayedIndex + m_nDisplayKLineCount, 
									m_kHighPrice, m_kLowPrice, m_volMax, m_interestMax);

		//	高低点间的波动幅度
		m_fPricePercentage = (m_kHighPrice - m_kLowPrice) /((m_kHighPrice + m_kLowPrice) / 2.0f);
	}
	else if(m_enRenderMode == enAxisMode)
	{
		if(m_nOpenIndex > m_pKLines->size() - 1) return;

		m_pKLines->GetPriceVolRange(1, m_pKLines->size() - 1, 
								m_kHighPrice, m_kLowPrice, m_volMax, m_interestMax);	

		// 以开盘为中轴价
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
			//	波动过小则统一使用 2% 的涨跌幅比例
			m_kHighPrice = m_kAxisPrice * (1 + fDefaultRange);
			m_kLowPrice = m_kAxisPrice * (1 - fDefaultRange);
			m_fPricePercentage = fDefaultRange;
		}
		else // 将幅度调的稍大，以便看到附近的均线 
		{
			m_fPricePercentage += 0.003f;
			m_kHighPrice = m_kAxisPrice * ( 1 + m_fPricePercentage);
			m_kLowPrice = m_kAxisPrice * ( 1 - m_fPricePercentage);
		}
	}

	if(m_bShowVol)
	{
		//	计算一个价格对应多少像素
		m_pixelPerPrice = (float)m_Rect.Height() * m_nKVolRatio / (m_nKVolRatio + 1) / (m_kHighPrice - m_kLowPrice);
		
		//	计算单位成交量对应多少像素
		m_pixelPerVol = ((float)m_Rect.Height()) / (m_nKVolRatio + 1) / m_volMax;

		m_pixelPerInterest = ((float)m_Rect.Height()) / (m_nKVolRatio + 1) / (2 * m_interestMax);
	}
	else
	{
		m_pixelPerPrice = (float)m_Rect.Height() / (m_kHighPrice - m_kLowPrice);
		m_pixelPerVol = 0;
	}

	//	计算K线的宽度
	m_kWidth = (m_Rect.Width() - LEFT_MARGIN - RIGHT_MARGIN - (m_nDisplayKLineCount + 1) * m_nKSpace)
		/(float)m_nDisplayKLineCount;
}

void KLineRenderer::CalculateKLineLayout(int nKIdx)
{
	/* 计算左右方向 */
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
		
		//	绘制20/60日高低点
		if(m_bShowHighLow)
			RenderHighLowPrice(pDC, i);

		/* 绘制均价线 */
		if(m_bShowAvg)
			RenderAvg(pDC, i);

		/* 绘制MA20/60 */
		if(m_bShowMA)
			RenderMA(pDC, i);

		/* 绘制15分钟，30分钟，60分钟时间线 */
		if(m_bShowCriticalTime)
			RenderCriticalTime(pDC, i);

		//	绘制K线
		RenderKLine(pDC, i);

		//	绘制交易记录
		if(m_pTradeRecords)
			RenderTradeRecord(pDC, i);

		//	绘制成交量/持仓量信息
		if(m_bShowVol)
			RenderVol(pDC, i);

		//	显示选中信息
		if(i == m_nSelectedIndex)
			RenderSelection(pDC, i);
	}

	//	绘制关键价格
	if(m_bShowKeyPrice)
		RenderKeyPrice(pDC);

	//	绘制当日最高最低点
	if(m_bShowMaxMin)
		RenderMaxMinPrice(pDC);

	//	绘制中轴线
	if(m_enRenderMode == enAxisMode)
		RenderAxis(pDC);

	//	显示价格百分比范围
	CString strPercent;
	strPercent.Format(_T("%.2f"), (m_fPricePercentage / 0.01));
	pDC->TextOutW(m_Rect.left + 5, m_Rect.top + 1, strPercent);
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

int KLineRenderer::GetMousePrice(CPoint pt)
{
	if(!m_bSelected) return 0;
	if(!m_Rect.PtInRect(pt)) return 0;

	m_cp = pt;

	return (m_kHighPrice - (pt.y - m_Rect.top) / m_pixelPerPrice);
}

void KLineRenderer::ToggleTrackingMode()
{
	if(!m_bSelected) return;

	if(m_enTrackingMode == enHighLowTMode)
	{
		m_enTrackingMode = enCloseTMode;
	}
	else if(m_enTrackingMode == enCloseTMode)
	{
		m_enTrackingMode = enMouseTMode;
	}
	else if(m_enTrackingMode == enMouseTMode)
	{
		m_enTrackingMode = enHighLowTMode;
	}
}

