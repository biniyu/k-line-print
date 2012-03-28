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
	m_pKLines = NULL;
	m_bSelected = false;
	m_enRenderMode = enHighLowMode;
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
}

void KLineRenderer::SelectByTime(int nTime)
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

	m_nStartIdx = m_nCurIdx - NEIGHBOR_KLINE_COUNT;
	m_nEndIdx = m_nCurIdx + NEIGHBOR_KLINE_COUNT;

	AdjustIndex();
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
	}
}

void KLineRenderer::SetKLineData(KLineCollection* pKLines)
{ 
	m_pKLines = pKLines; 

	m_nStartIdx = 0;
	m_nEndIdx = pKLines->size() - 1;
	m_nCurIdx = (m_nStartIdx + m_nEndIdx) /2;

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
}

void KLineRenderer::SwitchMode()
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
	//	��ͼ �� �ߵ͵�
	//	1��ͼ�� ���̼��ᣬ����ͼ
	//	5��ͼ�� �ߵ͵㣬����ͼ	

	float fPricePercentage, pixelPerPrice, pixelPerVol;

	if(!m_pKLines || m_pKLines->size() <= 1) return;

	if(m_bSelected)
		pDC->FillSolidRect(&m_Rect,RGB(220,220,220));
	else
		pDC->FillSolidRect(&m_Rect,RGB(255,255,255));

	int kHighPrice, kLowPrice, volMax, kAxisPrice;	//	ͼ����ʾ�ĸ�/�ͼ۷�Χ�������

	if(m_enRenderMode == enHighLowMode)
	{
		m_pKLines->GetPriceVolRange(m_nStartIdx, m_nEndIdx, 
								kHighPrice, kLowPrice, volMax);

		//	�ߵ͵��Ĳ�������
		fPricePercentage = (kHighPrice - kLowPrice) /((kHighPrice + kLowPrice) / 2.0f);
	}
	else if(m_enRenderMode == enAxisMode)
	{
		m_pKLines->GetPriceVolRange(1, m_pKLines->size() - 1, 
								kHighPrice, kLowPrice, volMax);	

		// �Կ���Ϊ�����
		kAxisPrice = (*m_pKLines)[1].open;
		int min = kLowPrice;
		int max = kHighPrice;

		if(max - kAxisPrice > kAxisPrice - min)
		{
			kHighPrice = kAxisPrice + (max - kAxisPrice);
			kLowPrice = kAxisPrice - (max - kAxisPrice);
		}
		else
		{
			kHighPrice = kAxisPrice + (kAxisPrice - min);
			kLowPrice = kAxisPrice - (kAxisPrice - min);
		}

		fPricePercentage = (kHighPrice - kAxisPrice)/(float)kAxisPrice;

		if( fPricePercentage < 0.02)
		{
			//	������С��ͳһʹ�� 2% ���ǵ�������
			kHighPrice = kAxisPrice * 1.02;
			kLowPrice = kAxisPrice * 0.98;
			fPricePercentage = 0.02;
		}
	}

	if(m_bShowVol)
	{
		//	����һ���۸��Ӧ��������
		pixelPerPrice = (float)m_Rect.Height() * m_nKVolRatio / (m_nKVolRatio + 1) / (kHighPrice - kLowPrice);
		
		//	���㵥λ�ɽ�����Ӧ��������
		pixelPerVol = ((float)m_Rect.Height()) / (m_nKVolRatio + 1) / volMax;
	}
	else
	{
		pixelPerPrice = (float)m_Rect.Height() / (kHighPrice - kLowPrice);
		pixelPerVol = 0;
	}

	//	����K�ߵĿ��
	float kWidth = (m_Rect.Width() - (m_nEndIdx - m_nStartIdx + 2) * m_nKSpace)
		/(float)(m_nEndIdx - m_nStartIdx + 1);

	CPen penRed, penGreen, penWhite, penGreyDotted, *pOldPen;

    penRed.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
    penGreen.CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
    penWhite.CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
    penGreyDotted.CreatePen(PS_DOT, 1, RGB(100, 100, 100));

	float timeLinePos = m_Rect.top + (kHighPrice - kLowPrice) * pixelPerPrice;

	//	���Ʒָ���
	pDC->MoveTo(m_Rect.left, timeLinePos);
	pDC->LineTo(m_Rect.right, timeLinePos);

	//	����ʱ����
	pDC->MoveTo(m_Rect.left, m_Rect.bottom);
	pDC->LineTo(m_Rect.right, m_Rect.bottom);

	//	����������
	if(m_enRenderMode == enAxisMode)
	{
		float axleLinePos = m_Rect.top + (kHighPrice - kAxisPrice) * pixelPerPrice;
		pDC->MoveTo(m_Rect.left, axleLinePos);
		pDC->LineTo(m_Rect.right, axleLinePos);
	}

	//	���Ƽ۸���
	pDC->MoveTo(m_Rect.left + 1, m_Rect.top);
	pDC->LineTo(m_Rect.left + 1, m_Rect.bottom);

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

		float kHighPos = m_Rect.top + (kHighPrice - kline.high) * pixelPerPrice;
		float kLowPos = m_Rect.top + (kHighPrice - kline.low) * pixelPerPrice;
		float kOpenPos = m_Rect.top + (kHighPrice - kline.open) * pixelPerPrice;
		float kClosePos = m_Rect.top + (kHighPrice - kline.close) * pixelPerPrice;
		
		float kAvgPos = m_Rect.top + (kHighPrice - kline.avg) * pixelPerPrice;
		float kMA20Pos = m_Rect.top + (kHighPrice - kline.ma20) * pixelPerPrice;
		float kMA60Pos = m_Rect.top + (kHighPrice - kline.ma60) * pixelPerPrice;

		/* ���ƾ����� */
		if(i > 0 && m_bShowAvg) 
		{
			pDC->MoveTo(kLastMiddle, kLastAvgPos);
			pDC->LineTo(kMiddle, kAvgPos);
		}

		if(m_bShowMA)
		{
			pDC->MoveTo(kLastMiddle, kLastMA20Pos);
			pDC->LineTo(kMiddle, kMA20Pos);

			pDC->MoveTo(kLastMiddle, kLastMA60Pos);
			pDC->LineTo(kMiddle, kMA60Pos);
		}

		kLastAvgPos = kAvgPos;
		kLastMA20Pos = kMA20Pos;
		kLastMA60Pos = kMA60Pos;

		kLastMiddle = kMiddle;

		//	���Ǻ죬�µ���
		if(kline.open > kline.close)
			pOldPen = pDC->SelectObject(&penGreen);
		else if(kline.open < kline.close)
			pOldPen = pDC->SelectObject(&penRed);
		else
			pOldPen = pDC->SelectObject(&penWhite);

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
			pDC->MoveTo(m_Rect.left, kClosePos);
			pDC->LineTo(kMiddle - kWidth * 2, kClosePos);

			pDC->MoveTo(kMiddle + kWidth * 2, kClosePos);
			pDC->LineTo(m_Rect.right, kClosePos);

			pDC->MoveTo(kMiddle, m_Rect.top);
			pDC->LineTo(kMiddle, kHighPos - 10);

			pDC->MoveTo(kMiddle, kLowPos + 10);
			pDC->LineTo(kMiddle, m_Rect.bottom);

			// ��ʾʱ��ͼ۸�

			CString strTime, strPrice;

			strTime.Format(_T("%d"), kline.time);
			strPrice.Format(_T("%d"), kline.close);

			pDC->TextOutW(kMiddle + 1, m_Rect.top + 1, strTime);
//			pDC->TextOutW(m_Rect.left + 1, kClosePos + 1, strPrice);

		}

		pDC->SelectObject(pOldPen);
	}

    penGreen.DeleteObject();
    penRed.DeleteObject();
    penWhite.DeleteObject();
	penGreyDotted.DeleteObject();
}
