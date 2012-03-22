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
	int kVolRatio = 3;						//	K图与成交量图的高度为3:1

	int kHighPrice, kLowPrice, kAxisPrice;	//	图上显示的高/低价范围，中轴价

	int kSpace = 2;							//	K线间的像素数

	if(klines.size() <= 1) return;

	//	当日的开盘价和最高最低价
	int open = klines[1].open;		//	
	int min = klines.low;
	int max = klines.high;

	// 以开盘为中轴价
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
		//	波动过小则统一使用 2% 的涨跌幅比例
		kHighPrice = kAxisPrice * 1.02;
		kLowPrice = kAxisPrice * 0.98;
		fGraphPercentage = 0.02;
	}
	else
	{
		// 如果幅度高于2%，按照实际幅度
		fGraphPercentage = fPricePercentage;
	}

	//	计算一个价格对应多少像素
	float pixelPerPrice = (float)rect.Height() * kVolRatio / (kVolRatio + 1) / (kHighPrice - kLowPrice);

	//	计算单位成交量对应多少像素
	float pixelPerVol = ((float)rect.Height()) / (kVolRatio + 1) / klines.maxvol;

	//	计算K线的宽度
	float kWidth = (rect.Width() - (klines.size() + 1) * kSpace)/(float)klines.size();

	CPen penRed, penGreen, penWhite, *pOldPen;

    penRed.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
    penGreen.CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
    penWhite.CreatePen(PS_SOLID, 1, RGB(0, 0, 255));

	float timeLinePos = rect.top + (kHighPrice - kLowPrice) * pixelPerPrice;
	float axleLinePos = rect.top + (kHighPrice - kAxisPrice) * pixelPerPrice;

	//	绘制分割线
	pDC->MoveTo(rect.left, timeLinePos);
	pDC->LineTo(rect.right, timeLinePos);

	//	绘制时间线
	pDC->MoveTo(rect.left, rect.bottom);
	pDC->LineTo(rect.right, rect.bottom);

	//	绘制中轴线
	pDC->MoveTo(rect.left, axleLinePos);
	pDC->LineTo(rect.right, axleLinePos);

	//	绘制价格线
//	pDC->MoveTo(1, rect.top);
//	pDC->LineTo(1, rect.bottom);

	CString strPercent;

	strPercent.Format(_T("%f"), (fGraphPercentage / 0.01));

	pDC->TextOutW(1,1,strPercent);

	float kLastAvgPos = 0;
	float kLastMiddle = 0;

	for(int i = 0; i < klines.size(); i++)
	{
		/* 计算左右方向 */
		float kLeft = rect.left + (i+1) * kSpace + kWidth * i;
		float kMiddle = kLeft + kWidth / 2;
		float kRight = kLeft + kWidth;

		KLine kline = klines[i];

		float kHighPos = rect.top + (kHighPrice - kline.high) * pixelPerPrice;
		float kLowPos = rect.top + (kHighPrice - kline.low) * pixelPerPrice;
		float kOpenPos = rect.top + (kHighPrice - kline.open) * pixelPerPrice;
		float kClosePos = rect.top + (kHighPrice - kline.close) * pixelPerPrice;
		float kAvgPos = rect.top + (kHighPrice - kline.avg) * pixelPerPrice;

		/* 绘制均价线 */
		if(i > 0) 
		{
			pDC->MoveTo(kLastMiddle, kLastAvgPos);
			pDC->LineTo(kMiddle, kAvgPos);
		}

		kLastAvgPos = kAvgPos;
		kLastMiddle = kMiddle;

		//	上涨红，下跌绿
		if(kline.open > kline.close)
			pOldPen = pDC->SelectObject(&penGreen);
		else if(kline.open < kline.close)
			pOldPen = pDC->SelectObject(&penRed);
		else
			pOldPen = pDC->SelectObject(&penWhite);

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

		//	成交量线

		float kVolPos = rect.bottom - kline.vol * pixelPerVol;

		pDC->MoveTo(kLeft, rect.bottom);
		pDC->LineTo(kLeft, kVolPos);
		pDC->LineTo(kRight, kVolPos);
		pDC->LineTo(kRight, rect.bottom);

		pDC->SelectObject(pOldPen);
	}

    penGreen.DeleteObject();
    penRed.DeleteObject();
    penWhite.DeleteObject();
}
