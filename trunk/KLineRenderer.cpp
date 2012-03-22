#include "StdAfx.h"
#include "KLineRenderer.h"
#include "KLineCollection.h"

KLineRenderer::KLineRenderer(void)
{
}

KLineRenderer::~KLineRenderer(void)
{
}

void KLineRenderer::Render(CDC* pDC, CRect rect, KLineCollection& klines)
{
	int kVolRatio = 3;						//	Kͼ��ɽ���ͼ�ĸ߶�Ϊ3:1

	int kHighPrice, kLowPrice, kAxisPrice;	//	ͼ����ʾ�ĸ�/�ͼ۷�Χ�������

	int kSpace = 2;							//	K�߼��������

	if(klines.size() <= 1) return;

	//	���յĿ��̼ۺ������ͼ�
	int open = klines[1].open;		//	
	int min = klines.low;
	int max = klines.high;

	// �Կ���Ϊ�����
	kAxisPrice = open;

	if(max - open > open - min)
	{
		kHighPrice = kAxisPrice + (max - open);
		kLowPrice = kAxisPrice - (max - open);
	}
	else
	{
		kHighPrice = kAxisPrice + (open - min);
		kLowPrice = kAxisPrice - (open - min);
	}

	float fPricePercentage = (kHighPrice - kAxisPrice)/(float)kAxisPrice;
	float fGraphPercentage;

	if( fPricePercentage < 0.02)
	{
		//	������С��ͳһʹ�� 2% ���ǵ�������
		kHighPrice = kAxisPrice * 1.02;
		kLowPrice = kAxisPrice * 0.98;
		fGraphPercentage = 0.02;
	}
	else
	{
		// ������ȸ���2%������ʵ�ʷ���
		fGraphPercentage = fPricePercentage;
	}

	//	����һ���۸��Ӧ��������
	float pixelPerPrice = (float)rect.Height() * kVolRatio / (kVolRatio + 1) / (kHighPrice - kLowPrice);

	//	���㵥λ�ɽ�����Ӧ��������
	float pixelPerVol = ((float)rect.Height()) / (kVolRatio + 1) / klines.maxvol;

	//	����K�ߵĿ��
	float kWidth = (rect.Width() - (klines.endidx - klines.startidx + 2) * kSpace)
		/(float)(klines.endidx - klines.startidx + 1);

	CPen penRed, penGreen, penWhite, penGreyDotted, *pOldPen;

    penRed.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
    penGreen.CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
    penWhite.CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
    penGreyDotted.CreatePen(PS_DOT, 1, RGB(100, 100, 100));

	float timeLinePos = rect.top + (kHighPrice - kLowPrice) * pixelPerPrice;
	float axleLinePos = rect.top + (kHighPrice - kAxisPrice) * pixelPerPrice;

	//	���Ʒָ���
	pDC->MoveTo(rect.left, timeLinePos);
	pDC->LineTo(rect.right, timeLinePos);

	//	����ʱ����
	pDC->MoveTo(rect.left, rect.bottom);
	pDC->LineTo(rect.right, rect.bottom);

	//	����������
	pDC->MoveTo(rect.left, axleLinePos);
	pDC->LineTo(rect.right, axleLinePos);

	//	���Ƽ۸���
	pDC->MoveTo(rect.left + 1, rect.top);
	pDC->LineTo(rect.left + 1, rect.bottom);

	CString strPercent;

	strPercent.Format(_T("%f"), (fGraphPercentage / 0.01));

	pDC->TextOutW(1,1,strPercent);

	float kLastAvgPos = 0;
	float kLastMiddle = 0;

	for(int i = klines.startidx; i <= klines.endidx; i++)
	{
		/* �������ҷ��� */
		float kLeft = rect.left + (i - klines.startidx + 1) * kSpace + kWidth * (i - klines.startidx);
		float kMiddle = kLeft + kWidth / 2;
		float kRight = kLeft + kWidth;

		KLine kline = klines[i];

		float kHighPos = rect.top + (kHighPrice - kline.high) * pixelPerPrice;
		float kLowPos = rect.top + (kHighPrice - kline.low) * pixelPerPrice;
		float kOpenPos = rect.top + (kHighPrice - kline.open) * pixelPerPrice;
		float kClosePos = rect.top + (kHighPrice - kline.close) * pixelPerPrice;
		float kAvgPos = rect.top + (kHighPrice - kline.avg) * pixelPerPrice;

		/* ���ƾ����� */
		if(i > 0) 
		{
//			pDC->MoveTo(kLastMiddle, kLastAvgPos);
//			pDC->LineTo(kMiddle, kAvgPos);
		}

		kLastAvgPos = kAvgPos;
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

		//	�ɽ�����

		float kVolPos = rect.bottom - kline.vol * pixelPerVol;

		pDC->MoveTo(kLeft, rect.bottom);
		pDC->LineTo(kLeft, kVolPos);
		pDC->LineTo(kRight, kVolPos);
		pDC->LineTo(kRight, rect.bottom);

		//	
		if(i == klines.curidx)
		{
			pDC->SelectObject(&penGreyDotted);
			pDC->MoveTo(rect.left, kClosePos);
			pDC->LineTo(rect.right, kClosePos);

			pDC->MoveTo(kMiddle, rect.top);
			pDC->LineTo(kMiddle, rect.bottom);
		}

		pDC->SelectObject(pOldPen);
	}

    penGreen.DeleteObject();
    penRed.DeleteObject();
    penWhite.DeleteObject();
	penGreyDotted.DeleteObject();
}
