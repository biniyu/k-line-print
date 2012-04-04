#include "StdAfx.h"
#include "KLineRenderer.h"
#include "KLineCollection.h"

#define ZOOM_STEP				20
#define NEIGHBOR_KLINE_COUNT	80

KLineRenderer::KLineRenderer(void)
{
	m_nKVolRatio = 3;						//	Kͼ��ɽ���ͼ�ĸ߶�Ϊ3:1
	m_nKSpace = 2;							//	K�߼��������
	m_bShowVol = true;
	m_bShowAvg = false;
	m_bShowMA = false;
	m_bShowCriticalTime = false;
	m_pKLines = NULL;
	m_bSelected = false;
	m_enRenderMode = enHighLowMode;
	m_enTrackingMode = enCloseTMode;
	m_nOpenIndex = -1;
	m_bShowDate = false;
	m_nStartIdx = m_nCurIdx = m_nEndIdx = 0;
}

KLineRenderer::~KLineRenderer(void)
{
}

int KLineRenderer::GetCurTime()
{
	if(!m_pKLines) return 0;
	return (*m_pKLines)[m_nCurIdx].time;
}

void KLineRenderer::AdjustIndex()
{
	if(m_nStartIdx < 0) m_nStartIdx = 0;
	if(m_nEndIdx > m_pKLines->size() - 1) m_nEndIdx = m_pKLines->size() - 1;

	if(m_nCurIdx < m_nStartIdx) m_nCurIdx = m_nStartIdx;
	if(m_nCurIdx > m_nEndIdx) m_nCurIdx = m_nEndIdx;
}

void KLineRenderer::SelectByTime(int nTime, bool bKeepScale)
{
	if(!m_pKLines) return;

	for(int i = 0; i < m_pKLines->size(); i++)
	{
		if((*m_pKLines)[i].time >= nTime) 
		{
			m_nCurIdx = i;
			break;
		}
	}

	if(!bKeepScale)
	{
		m_nStartIdx = m_nCurIdx - NEIGHBOR_KLINE_COUNT;
		m_nEndIdx = m_nCurIdx + NEIGHBOR_KLINE_COUNT;

		AdjustIndex();
	}

	m_nSelectedPrice = (*m_pKLines)[m_nCurIdx].close;
}

void KLineRenderer::Select(CPoint pt)
{
	if(m_Rect.PtInRect(pt))
		m_bSelected = true;
	else
		m_bSelected = false;

	// TODO : ѡ��K��

	if(m_bSelected)
	{
		//	����K�ߵĿ��
		float kWidth = (m_Rect.Width() - (m_nEndIdx - m_nStartIdx + 2) * m_nKSpace)
			/(float)(m_nEndIdx - m_nStartIdx + 1);

		m_nCurIdx = (pt.x - m_Rect.left) / (kWidth + m_nKSpace) + m_nStartIdx;
		m_nSelectedPrice = (*m_pKLines)[m_nCurIdx].close;
	}
}

void KLineRenderer::SetKLineData(KLineCollection* pKLines)
{ 
	m_pKLines = pKLines; 

	m_nStartIdx = 0;
	m_nEndIdx = pKLines->size() - 1;
//	m_nCurIdx = (m_nStartIdx + m_nEndIdx) /2;

	AdjustIndex();
}

void KLineRenderer::ZoomIn()
{
	if(!m_pKLines || !m_bSelected) return;

	int s1 = m_nCurIdx - m_nStartIdx;
	int s2 = m_nEndIdx - m_nCurIdx;

	m_nStartIdx += ZOOM_STEP * s1 / (float) (s1+s2);
	m_nEndIdx -= ZOOM_STEP * s2 / (float) (s1+s2);

	AdjustIndex();
}

void KLineRenderer::ZoomOut()
{
	if(!m_pKLines || !m_bSelected) return;

	int s1 = m_nCurIdx - m_nStartIdx;
	int s2 = m_nEndIdx - m_nCurIdx;

	m_nStartIdx -= ZOOM_STEP * s1 / (float) (s1+s2);
	m_nEndIdx += ZOOM_STEP * s2 / (float) (s1+s2);

	AdjustIndex();
}

void KLineRenderer::MovePrev()
{
	if(!m_pKLines || !m_bSelected) return;

	if(m_nCurIdx > 0)
		m_nCurIdx--;

	if(m_nCurIdx < m_nStartIdx)
	{
		m_nEndIdx -= ( m_nStartIdx - m_nCurIdx);
		m_nStartIdx = m_nCurIdx;
	}

	m_nSelectedPrice = (*m_pKLines)[m_nCurIdx].close;
}

void KLineRenderer::MoveNext()
{
	if(!m_pKLines || !m_bSelected) return;

	if(m_nCurIdx < m_pKLines->size() - 1)
		m_nCurIdx++;

	if(m_nCurIdx > m_nEndIdx)
	{
		m_nStartIdx += ( m_nCurIdx - m_nEndIdx);
		m_nEndIdx = m_nCurIdx;
	}

	m_nSelectedPrice = (*m_pKLines)[m_nCurIdx].close;
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
	float fPricePercentage, pixelPerVol;
	int kLowPrice, volMax, kAxisPrice;	//	ͼ����ʾ�ĸ�/�ͼ۷�Χ�������

	if(!m_pKLines || m_pKLines->size() <= 1) return;

	if(m_bSelected)
		pDC->FillSolidRect(&m_Rect,RGB(230,230,230));
	else
		pDC->FillSolidRect(&m_Rect,RGB(255,255,255));

	if(m_enRenderMode == enHighLowMode)
	{
		m_pKLines->GetPriceVolRange(m_nStartIdx, m_nEndIdx, 
								m_kHighPrice, kLowPrice, volMax);

		//	�ߵ͵��Ĳ�������
		fPricePercentage = (m_kHighPrice - kLowPrice) /((m_kHighPrice + kLowPrice) / 2.0f);
	}
	else if(m_enRenderMode == enAxisMode)
	{
		m_pKLines->GetPriceVolRange(1, m_pKLines->size() - 1, 
								m_kHighPrice, kLowPrice, volMax);	

		// �Կ���Ϊ�����
		kAxisPrice = (*m_pKLines)[1].open;
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

		if( fPricePercentage < 0.02)
		{
			//	������С��ͳһʹ�� 2% ���ǵ�������
			m_kHighPrice = kAxisPrice * 1.02;
			kLowPrice = kAxisPrice * 0.98;
			fPricePercentage = 0.02;
		}
		else // �����ȵ����Դ��Ա㿴�������ľ��� 
		{
			fPricePercentage += 0.003;
			m_kHighPrice = kAxisPrice * ( 1 + fPricePercentage);
			kLowPrice = kAxisPrice * ( 1 - fPricePercentage);
		}
	}

	if(m_bShowVol)
	{
		//	����һ���۸��Ӧ��������
		m_pixelPerPrice = (float)m_Rect.Height() * m_nKVolRatio / (m_nKVolRatio + 1) / (m_kHighPrice - kLowPrice);
		
		//	���㵥λ�ɽ�����Ӧ��������
		pixelPerVol = ((float)m_Rect.Height()) / (m_nKVolRatio + 1) / volMax;
	}
	else
	{
		m_pixelPerPrice = (float)m_Rect.Height() / (m_kHighPrice - kLowPrice);
		pixelPerVol = 0;
	}

	//	����K�ߵĿ��
	float kWidth = (m_Rect.Width() - (m_nEndIdx - m_nStartIdx + 2) * m_nKSpace)
		/(float)(m_nEndIdx - m_nStartIdx + 1);

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

	//	���Ʒָ���
	pDC->MoveTo(m_Rect.left, timeLinePos);
	pDC->LineTo(m_Rect.right, timeLinePos);

	//	����ʱ����
	pDC->MoveTo(m_Rect.left, m_Rect.bottom);
	pDC->LineTo(m_Rect.right, m_Rect.bottom);

	//	����������
	if(m_enRenderMode == enAxisMode)
	{
		float axleLinePos = m_Rect.top + (m_kHighPrice - kAxisPrice) * m_pixelPerPrice;
		pDC->MoveTo(m_Rect.left, axleLinePos);
		pDC->LineTo(m_Rect.right, axleLinePos);
	}

	//	���Ƽ۸���
	pDC->MoveTo(m_Rect.left + 1, m_Rect.top);
	pDC->LineTo(m_Rect.left + 1, m_Rect.bottom);

	//	���ƹؼ��۸���
	map<int, string>::iterator itKeyPrice;
	map<int, string>& keyPrices = m_pKLines->GetKeyPrices();

	pOldPen = pDC->SelectObject(&penGreenDotted);

	for(itKeyPrice = keyPrices.begin(); itKeyPrice != keyPrices.end(); itKeyPrice++)
	{
		if(itKeyPrice->first > m_kHighPrice || itKeyPrice->first < kLowPrice) 
			continue;

		float keyPricePos = m_Rect.top + (m_kHighPrice - itKeyPrice->first) * m_pixelPerPrice;

		pDC->MoveTo(m_Rect.left, keyPricePos);
		pDC->LineTo(m_Rect.right, keyPricePos);	
		pDC->TextOutW(m_Rect.left + 10, keyPricePos - 5, CString(itKeyPrice->second.c_str()));
	}

	pDC->SelectObject(pOldPen);

	//////////////////////////////////////////////////////////////////////////////////

	CString strPercent;

	strPercent.Format(_T("%f"), (fPricePercentage / 0.01));

	pDC->TextOutW(m_Rect.left + 1, m_Rect.top + 1, strPercent);

	float kLastAvgPos = 0;
	float kLastMA20Pos = 0;
	float kLastMA60Pos = 0;

	float kLastMiddle = 0;

	for(int i = m_nStartIdx; i <= m_nEndIdx; i++)
	{
		/* �������ҷ��� */
		float kLeft = m_Rect.left + (i - m_nStartIdx + 1) * m_nKSpace + kWidth * (i - m_nStartIdx);
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

		/* ���ƾ����� */
		if(i > m_nStartIdx + 1 && m_bShowAvg) 
		{
			pDC->MoveTo(kLastMiddle, kLastAvgPos);
			pDC->LineTo(kMiddle, kAvgPos);
		}

		if(i > m_nStartIdx + 1 && m_bShowMA)
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

		/* ����15���ӣ�30���ӣ�60����ʱ���� */

		int tmpHour = kline.time / 10000;
		int tmpMinute =  kline.time % 10000 / 100;
		int tmpSecond =  kline.time % 10000 % 100;
		
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
		}

		//	���Ǻ죬�µ���
		if(kline.open > kline.close)
			pDC->SelectObject(&penGreen);
		else if(kline.open < kline.close)
			pDC->SelectObject(&penRed);
		else
			pDC->SelectObject(&penWhite);

		//	ʵ����
		pDC->MoveTo(kLeft, kOpenPos);
		pDC->LineTo(kLeft, kClosePos);
		pDC->LineTo(kRight, kClosePos);
		pDC->LineTo(kRight, kOpenPos);
		pDC->LineTo(kLeft, kOpenPos);

		//	Ӱ��
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
			//	�ɽ�����
			float kVolPos = m_Rect.bottom - kline.vol * pixelPerVol;
			pDC->MoveTo(kLeft, m_Rect.bottom);
			pDC->LineTo(kLeft, kVolPos);
			pDC->LineTo(kRight, kVolPos);
			pDC->LineTo(kRight, m_Rect.bottom);
		}

		//	
		if(i == m_nCurIdx)
		{
			pDC->SelectObject(&penGreyDotted);

			if(!m_bSelected)	//	δѡ��
			{
				float kCurPos = m_Rect.top + (m_kHighPrice - m_nSelectedPrice) * m_pixelPerPrice;

				pDC->MoveTo(m_Rect.left, kCurPos);
				pDC->LineTo(kMiddle - kWidth * 2, kCurPos);

				pDC->MoveTo(kMiddle + kWidth * 2, kCurPos);
				pDC->LineTo(m_Rect.right, kCurPos);

				pDC->MoveTo(kMiddle, m_Rect.top);
				pDC->LineTo(kMiddle, kHighPos - 10);

				pDC->MoveTo(kMiddle, kLowPos + 10);
				pDC->LineTo(kMiddle, m_Rect.bottom);
			}
			else	//	ѡ��
			{
				if(m_enTrackingMode == enCloseTMode)
				{
					float kCurPos = m_Rect.top + (m_kHighPrice - kline.close) * m_pixelPerPrice;

					pDC->MoveTo(m_Rect.left, kCurPos);
					pDC->LineTo(kMiddle - kWidth * 2, kCurPos);

					pDC->MoveTo(kMiddle + kWidth * 2, kCurPos);
					pDC->LineTo(m_Rect.right, kCurPos);

					pDC->MoveTo(kMiddle, m_Rect.top);
					pDC->LineTo(kMiddle, kHighPos - 10);

					pDC->MoveTo(kMiddle, kLowPos + 10);
					pDC->LineTo(kMiddle, m_Rect.bottom);
				}
				else if(m_enTrackingMode == enHighLowTMode)
				{
					float kCurHighPos = m_Rect.top + (m_kHighPrice - kline.high) * m_pixelPerPrice;
					float kCurLowPos = m_Rect.top + (m_kHighPrice - kline.low) * m_pixelPerPrice;

					pDC->MoveTo(m_Rect.left, kCurHighPos);
					pDC->LineTo(kMiddle - kWidth * 2, kCurHighPos);
					pDC->MoveTo(kMiddle + kWidth * 2, kCurHighPos);
					pDC->LineTo(m_Rect.right, kCurHighPos);

					pDC->MoveTo(m_Rect.left, kCurLowPos);
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
					//	���Ƶ�ǰ�������λ�õĸ�����
					pDC->MoveTo(m_Rect.left, m_cp.y);
					pDC->LineTo(m_Rect.right, m_cp.y);

					pDC->MoveTo(kMiddle, m_Rect.top);
					pDC->LineTo(kMiddle, kHighPos - 10);
					pDC->MoveTo(kMiddle, kLowPos + 10);
					pDC->LineTo(kMiddle, m_Rect.bottom);
				}
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
				// w=y+[y/4]+[c/4]-2c+[26(m+1)/10]+d-1
				int orgdate = kline.time;
				int c = orgdate / 1000000;
				int y = orgdate % 1000000 / 10000;
				int m = orgdate % 1000000 % 10000 / 100;
				int d = orgdate % 1000000 % 10000 % 100;

				if(m <= 2)
				{
					m += 12;
					y -= 1;
				}

				int weekday = (y + y/4 + c/4 - 2*c + 26*(m+1)/10 + d-1) % 7;
				strTime.Format(_T("%d - ����%d - %s"), kline.time, weekday, strCur);				
			}
			else
			{
				strTime.Format(_T("%d [%s] %s vol:%d vol_acc:%d"), 
							kline.time, strOpen, strCur, 
							kline.vol, kline.vol_acc);
			}

			pDC->TextOutW(kMiddle + 1, m_Rect.top + 1, strTime);
		}

		pDC->SelectObject(pOldPen);
	}

    penGreen.DeleteObject();
    penRed.DeleteObject();
    penWhite.DeleteObject();
	penGreyDotted.DeleteObject();

	penRedDotted.DeleteObject();
    penGreenDotted.DeleteObject();
    penBlueDotted.DeleteObject();

}

void KLineRenderer::SetSelectedPrice(int price)
{
	m_nSelectedPrice = price;
}

int KLineRenderer::GetSelectedClosePrice()
{
	return (*m_pKLines)[m_nCurIdx].close;
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

