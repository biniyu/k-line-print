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
}

KLineRenderer::~KLineRenderer(void)
{
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
		float kWidth = (m_Rect.Width() - LEFT_MARGIN - RIGHT_MARGIN - (m_nDisplayKLineCount + 1) * m_nKSpace)
			/(float)m_nDisplayKLineCount;

		m_nSelectedIndex = (pt.x - (m_Rect.left + LEFT_MARGIN)) / (kWidth + m_nKSpace) + m_nFirstDisplayedIndex;

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

void KLineRenderer::Render(CDC* pDC)
{
	CFont font;
	float fPricePercentage, pixelPerVol;
	int kLowPrice, volMax, kAxisPrice;	//	图上显示的高/低价范围，中轴价

	if(!m_pKLines || !m_pKLines->size()) return;

	if(m_Rect == CRect(0,0,0,0)) return;

	font.CreatePointFont(100, CString("Tahoma"));
	pDC->SelectObject(&font);

	if(m_bSelected)
		pDC->FillSolidRect(&m_Rect,RGB(230,230,230));
	else
		pDC->FillSolidRect(&m_Rect,RGB(255,255,255));

	if(m_enRenderMode == enHighLowMode)
	{
		m_pKLines->GetPriceVolRange(m_nFirstDisplayedIndex, 
									m_nFirstDisplayedIndex + m_nDisplayKLineCount, 
									m_kHighPrice, kLowPrice, volMax);

		//	高低点间的波动幅度
		fPricePercentage = (m_kHighPrice - kLowPrice) /((m_kHighPrice + kLowPrice) / 2.0f);
	}
	else if(m_enRenderMode == enAxisMode)
	{
		if(m_nOpenIndex > m_pKLines->size() - 1) return;

		m_pKLines->GetPriceVolRange(1, m_pKLines->size() - 1, 
								m_kHighPrice, kLowPrice, volMax);	

		// 以开盘为中轴价
		kAxisPrice = (*m_pKLines)[m_nOpenIndex].open;
		int min = kLowPrice;
		int max = m_kHighPrice;

		if(max - kAxisPrice > kAxisPrice - min)
		{
			m_kHighPrice = kAxisPrice + (max - kAxisPrice);
			kLowPrice = kAxisPrice - (max - kAxisPrice);
		}
		else
		{
			m_kHighPrice = kAxisPrice + (kAxisPrice - min);
			kLowPrice = kAxisPrice - (kAxisPrice - min);
		}

		fPricePercentage = (m_kHighPrice - kAxisPrice)/(float)kAxisPrice;

		float fDefaultRange = 0.02;

		if( fPricePercentage < fDefaultRange)
		{
			//	波动过小则统一使用 2% 的涨跌幅比例
			m_kHighPrice = kAxisPrice * (1 + fDefaultRange);
			kLowPrice = kAxisPrice * (1 - fDefaultRange);
			fPricePercentage = fDefaultRange;
		}
		else // 将幅度调的稍大，以便看到附近的均线 
		{
			fPricePercentage += 0.003;
			m_kHighPrice = kAxisPrice * ( 1 + fPricePercentage);
			kLowPrice = kAxisPrice * ( 1 - fPricePercentage);
		}
	}

	if(m_bShowVol)
	{
		//	计算一个价格对应多少像素
		m_pixelPerPrice = (float)m_Rect.Height() * m_nKVolRatio / (m_nKVolRatio + 1) / (m_kHighPrice - kLowPrice);
		
		//	计算单位成交量对应多少像素
		pixelPerVol = ((float)m_Rect.Height()) / (m_nKVolRatio + 1) / volMax;
	}
	else
	{
		m_pixelPerPrice = (float)m_Rect.Height() / (m_kHighPrice - kLowPrice);
		pixelPerVol = 0;
	}

	//	计算K线的宽度
	float kWidth = (m_Rect.Width() - LEFT_MARGIN - RIGHT_MARGIN - (m_nDisplayKLineCount + 1) * m_nKSpace)
		/(float)m_nDisplayKLineCount;

	CPen penRed, penGreen, penWhite, penGreyDotted, *pOldPen = 0;
	CPen penRedDotted, penGreenDotted, penBlueDotted;

    penRed.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
    penGreen.CreatePen(PS_SOLID, 1, RGB(0, 180, 0));
    penWhite.CreatePen(PS_SOLID, 1, RGB(0, 0, 255));

	penGreyDotted.CreatePen(PS_DOT, 1, RGB(100, 100, 100));
    
	penRedDotted.CreatePen(PS_DASH, 1, RGB(255, 0, 0));
    penGreenDotted.CreatePen(PS_DASH, 1, RGB(0, 180, 0));
    penBlueDotted.CreatePen(PS_DASH, 1, RGB(0, 0, 255));

	float timeLinePos = m_Rect.top + (m_kHighPrice - kLowPrice) * m_pixelPerPrice;

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

	//////////////////////////////////////////////////////////////////////////////////
	float kLastAvgPos = 0;
	float kLastMA20Pos = 0;
	float kLastMA60Pos = 0;

	float kLastMiddle = 0;

	for(int i = m_nFirstDisplayedIndex; i <= m_nFirstDisplayedIndex + m_nDisplayKLineCount; i++)
	{
		if( i >= m_pKLines->size()) break;

		/* 计算左右方向 */
		float kLeft = m_Rect.left + LEFT_MARGIN + (i - m_nFirstDisplayedIndex + 1) * m_nKSpace
								+ kWidth * (i - m_nFirstDisplayedIndex);

		float kMiddle = kLeft + kWidth / 2;
		float kRight = kLeft + kWidth;

		KLine kline = (*m_pKLines)[i];

		float kHighPos = m_Rect.top + (m_kHighPrice - kline.high) * m_pixelPerPrice;
		float kLowPos = m_Rect.top + (m_kHighPrice - kline.low) * m_pixelPerPrice;
		float kOpenPos = m_Rect.top + (m_kHighPrice - kline.open) * m_pixelPerPrice;
		float kClosePos = m_Rect.top + (m_kHighPrice - kline.close) * m_pixelPerPrice;
		
		float kAvgPos = m_Rect.top + (m_kHighPrice - kline.avg) * m_pixelPerPrice;
		float kMA20Pos = m_Rect.top + (m_kHighPrice - kline.ma20) * m_pixelPerPrice;
		float kMA60Pos = m_Rect.top + (m_kHighPrice - kline.ma60) * m_pixelPerPrice;

		float kH20Pos = m_Rect.top + (m_kHighPrice - kline.high20) * m_pixelPerPrice;
		float kH60Pos = m_Rect.top + (m_kHighPrice - kline.high60) * m_pixelPerPrice;

		float kL20Pos = m_Rect.top + (m_kHighPrice - kline.low20) * m_pixelPerPrice;
		float kL60Pos = m_Rect.top + (m_kHighPrice - kline.low60) * m_pixelPerPrice;

		//	绘制20/60日高低点
		if(m_bShowHighLow)
		{
			pOldPen = pDC->SelectObject(&penWhite);

			if(kH20Pos < m_Rect.bottom && kH20Pos > m_Rect.top)
			{
				pDC->MoveTo(kLeft - m_nKSpace / 2, kH20Pos);
				pDC->LineTo(kRight + m_nKSpace / 2, kH20Pos);
			}

			if(kL20Pos < m_Rect.bottom && kL20Pos > m_Rect.top)
			{
				pDC->MoveTo(kLeft - m_nKSpace / 2, kL20Pos);
				pDC->LineTo(kRight + m_nKSpace / 2, kL20Pos);
			}

			pDC->SelectObject(&penRed);

			if(kH60Pos < m_Rect.bottom && kH60Pos > m_Rect.top)
			{
				pDC->MoveTo(kLeft  - m_nKSpace / 2, kH60Pos);
				pDC->LineTo(kRight + m_nKSpace / 2, kH60Pos);
			}

			if(kL60Pos < m_Rect.bottom && kL60Pos > m_Rect.top)
			{
				pDC->MoveTo(kLeft  - m_nKSpace / 2, kL60Pos);
				pDC->LineTo(kRight + m_nKSpace / 2, kL60Pos);
			}

			pDC->SelectObject(pOldPen);
		}

		/* 绘制均价线 */
		if(i > m_nFirstDisplayedIndex + 1 && m_bShowAvg) 
		{
			pDC->MoveTo(kLastMiddle, kLastAvgPos);
			pDC->LineTo(kMiddle, kAvgPos);
		}

		if(i > m_nFirstDisplayedIndex + 1 && m_bShowMA)
		{
			if(kLastMA20Pos > m_Rect.top && kLastMA20Pos < m_Rect.bottom)
			{
				pDC->MoveTo(kLastMiddle, kLastMA20Pos);
				pDC->LineTo(kMiddle, kMA20Pos);
			}

			if(kLastMA60Pos > m_Rect.top && kLastMA60Pos < m_Rect.bottom)
			{
				pDC->MoveTo(kLastMiddle, kLastMA60Pos);
				pDC->LineTo(kMiddle, kMA60Pos);
			}
		}

		kLastAvgPos = kAvgPos;
		kLastMA20Pos = kMA20Pos;
		kLastMA60Pos = kMA60Pos;

		kLastMiddle = kMiddle;

		/* 绘制15分钟，30分钟，60分钟时间线 */

		int tmpHour = kline.time / 1000 / 3600;
		int tmpMinute =  kline.time / 1000 % 3600 / 60;
		int tmpSecond =  kline.time / 1000 % 3600 % 60;
		
		int tmpTime = tmpHour * 60 + tmpMinute;

		if(tmpTime % 30 == 0)
		{
			pOldPen = pDC->SelectObject(&penRedDotted);		
		}
		else if(tmpTime % 15 == 0)
		{
			pOldPen = pDC->SelectObject(&penBlueDotted);
		}
		else
		{
			pOldPen = pDC->SelectObject(&penBlueDotted);
		}

		if(tmpTime % 15 == 0 && m_bShowCriticalTime)
		{		
			pDC->MoveTo(kMiddle, m_Rect.top);
			pDC->LineTo(kMiddle, kHighPos - 10);

			pDC->MoveTo(kMiddle, kLowPos + 10);
			pDC->LineTo(kMiddle, timeLinePos);

			CString tmptime;

			tmptime.Format(_T("%02d:%02d"), tmpHour, tmpMinute);

			CSize sz = pDC->GetTextExtent(tmptime);
			pDC->TextOutW(kMiddle, timeLinePos - sz.cy, tmptime);
		}

		//	上涨红，下跌绿
		if(kline.open > kline.close)
			pDC->SelectObject(&penGreen);
		else if(kline.open < kline.close)
			pDC->SelectObject(&penRed);
		else
			pDC->SelectObject(&penWhite);

		//	实体线
		pDC->MoveTo(kLeft, kOpenPos);
		pDC->LineTo(kLeft, kClosePos);
		pDC->LineTo(kRight, kClosePos);
		pDC->LineTo(kRight, kOpenPos);
		pDC->LineTo(kLeft, kOpenPos);

		//	影线
		if(kline.open > kline.close)
		{
			pDC->MoveTo(kMiddle, kHighPos);
			pDC->LineTo(kMiddle, kOpenPos);
			pDC->MoveTo(kMiddle, kClosePos);
			pDC->LineTo(kMiddle, kLowPos);
		}
		else
		{
			pDC->MoveTo(kMiddle, kHighPos);
			pDC->LineTo(kMiddle, kClosePos);
			pDC->MoveTo(kMiddle, kOpenPos);
			pDC->LineTo(kMiddle, kLowPos);
		}

		if(m_bShowVol)
		{
			//	成交量线
			float kVolPos = m_Rect.bottom - kline.vol * pixelPerVol;
			pDC->MoveTo(kLeft, m_Rect.bottom);
			pDC->LineTo(kLeft, kVolPos);
			pDC->LineTo(kRight, kVolPos);
			pDC->LineTo(kRight, m_Rect.bottom);
		}

		//	
		if(i == m_nSelectedIndex)
		{
			pDC->SelectObject(&penGreyDotted);

			if(!m_bSelected)	//	未选中
			{
				float kCurPos = m_Rect.top + (m_kHighPrice - m_nSelectedPrice) * m_pixelPerPrice;

				pDC->MoveTo(m_Rect.left + LEFT_MARGIN, kCurPos);
				pDC->LineTo(kMiddle - kWidth * 2, kCurPos);

				pDC->MoveTo(kMiddle + kWidth * 2, kCurPos);
				pDC->LineTo(m_Rect.right, kCurPos);

				pDC->MoveTo(kMiddle, m_Rect.top);
				pDC->LineTo(kMiddle, kHighPos - 10);

				pDC->MoveTo(kMiddle, kLowPos + 10);
				pDC->LineTo(kMiddle, m_Rect.bottom);
			}
			else	//	选中
			{
				if(m_enTrackingMode == enCloseTMode)
				{
					float kCurPos = m_Rect.top + (m_kHighPrice - kline.close) * m_pixelPerPrice;

					pDC->MoveTo(m_Rect.left + LEFT_MARGIN, kCurPos);
					pDC->LineTo(kMiddle - kWidth * 2, kCurPos);

					pDC->MoveTo(kMiddle + kWidth * 2, kCurPos);
					pDC->LineTo(m_Rect.right - RIGHT_MARGIN, kCurPos);

					pDC->MoveTo(kMiddle, m_Rect.top);
					pDC->LineTo(kMiddle, kHighPos - 10);

					pDC->MoveTo(kMiddle, kLowPos + 10);
					pDC->LineTo(kMiddle, m_Rect.bottom);

					CString tmp;

					tmp.Format(_T("%d"), kline.close);

					pDC->TextOutW(m_Rect.right - RIGHT_MARGIN + 4, kCurPos, tmp);
				}
				else if(m_enTrackingMode == enHighLowTMode)
				{
					float kCurHighPos = m_Rect.top + (m_kHighPrice - kline.high) * m_pixelPerPrice;
					float kCurLowPos = m_Rect.top + (m_kHighPrice - kline.low) * m_pixelPerPrice;

					pDC->MoveTo(m_Rect.left + LEFT_MARGIN, kCurHighPos);
					pDC->LineTo(kMiddle - kWidth * 2, kCurHighPos);
					pDC->MoveTo(kMiddle + kWidth * 2, kCurHighPos);
					pDC->LineTo(m_Rect.right, kCurHighPos);

					pDC->MoveTo(m_Rect.left + LEFT_MARGIN, kCurLowPos);
					pDC->LineTo(kMiddle - kWidth * 2, kCurLowPos);
					pDC->MoveTo(kMiddle + kWidth * 2, kCurLowPos);
					pDC->LineTo(m_Rect.right, kCurLowPos);

					pDC->MoveTo(kMiddle, m_Rect.top);
					pDC->LineTo(kMiddle, kHighPos - 10);
					pDC->MoveTo(kMiddle, kLowPos + 10);
					pDC->LineTo(kMiddle, m_Rect.bottom);
				}
				else if(m_enTrackingMode == enMouseTMode)
				{
					//	绘制当前鼠标所在位置的跟踪线
					pDC->MoveTo(m_Rect.left + LEFT_MARGIN, m_cp.y);
					pDC->LineTo(m_Rect.right, m_cp.y);

					pDC->MoveTo(kMiddle, m_Rect.top);
					pDC->LineTo(kMiddle, kHighPos - 10);
					pDC->MoveTo(kMiddle, kLowPos + 10);
					pDC->LineTo(kMiddle, m_Rect.bottom);
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

			if(kMiddle + 1 + sz.cx > m_Rect.right)
			{
				pDC->TextOutW(kMiddle - sz.cx - 5, m_Rect.top + 1, strTime);
			}
			else
			{
				pDC->TextOutW(kMiddle + 5, m_Rect.top + 1, strTime);
			}
		}

		pDC->SelectObject(pOldPen);
	}

	//	绘制关键价格线(文本需要错开，以免互相覆盖)
	map<int, string>::iterator itKeyPrice;
	map<int, string>& keyPrices = m_pKLines->GetKeyPrices();

	pOldPen = pDC->SelectObject(&penGreenDotted);

	for(itKeyPrice = keyPrices.begin(); itKeyPrice != keyPrices.end(); itKeyPrice++)
	{
		if(itKeyPrice->first > m_kHighPrice || itKeyPrice->first < kLowPrice) 
			continue;

		float keyPricePos = m_Rect.top + (m_kHighPrice - itKeyPrice->first) * m_pixelPerPrice;

		pDC->MoveTo(m_Rect.left + LEFT_MARGIN, keyPricePos);
		pDC->LineTo(m_Rect.right - RIGHT_MARGIN, keyPricePos);	
		pDC->TextOutW(m_Rect.left + 5, keyPricePos - 5, CString(itKeyPrice->second.c_str()));
	}

	//	绘制当日最高最低点

	if(m_bShowMaxMin)
	{
		float maxPricePos = m_Rect.top + (m_kHighPrice - m_pKLines->m_nMaxPrice) * m_pixelPerPrice;
		float minPricePos = m_Rect.top + (m_kHighPrice - m_pKLines->m_nMinPrice) * m_pixelPerPrice;

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

	pDC->SelectObject(pOldPen);

	CString strPercent;

	//	绘制中轴线
	if(m_enRenderMode == enAxisMode)
	{
		float axleLinePos = m_Rect.top + (m_kHighPrice - kAxisPrice) * m_pixelPerPrice;
		pDC->MoveTo(m_Rect.left + LEFT_MARGIN, axleLinePos);
		pDC->LineTo(m_Rect.right - RIGHT_MARGIN, axleLinePos);

		//	显示跳空幅度
		float todayOpen = (*m_pKLines)[m_nOpenIndex].open;
		float prevClose = (*m_pKLines)[0].close;
		float gap = (todayOpen - prevClose) / prevClose * 100;
		int range = m_pKLines->m_nMaxPrice - m_pKLines->m_nMinPrice;

		KLine tmpk = (*m_pKLines)[m_nSelectedIndex];

		strPercent.Format(_T("图%.2f%% 跳%.2f%% 波幅%d点 均偏%d点"), 
					(fPricePercentage / 0.01), gap, range, tmpk.avg_devi);

		pDC->TextOutW(m_Rect.left + LEFT_MARGIN + 10, m_Rect.top + 20, strPercent);
	}

	strPercent.Format(_T("%.2f"), (fPricePercentage / 0.01));
	pDC->TextOutW(m_Rect.left + 5, m_Rect.top + 1, strPercent);

    penGreen.DeleteObject();
    penRed.DeleteObject();
    penWhite.DeleteObject();
	penGreyDotted.DeleteObject();

	penRedDotted.DeleteObject();
    penGreenDotted.DeleteObject();
    penBlueDotted.DeleteObject();
	font.DeleteObject();
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

