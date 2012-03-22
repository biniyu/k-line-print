#pragma once

class KLineCollection;

class KLineRenderer
{
public:
	KLineRenderer(void);
	~KLineRenderer(void);

	void Render(CDC* pDC, CRect rect, KLineCollection& kline);
};
