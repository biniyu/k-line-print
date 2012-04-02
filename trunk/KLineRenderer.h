#pragma once

class KLineCollection;

class KLineRenderer
{
public:

	enum RenderMode
	{
		enHighLowMode,
		enAxisMode
	};

	enum TrackingMode
	{
		enHighLowTMode,
		enCloseTMode,
		enMouseTMode
	};

private:

	RenderMode m_enRenderMode;

	TrackingMode m_enTrackingMode;

	//	开始显示的K线索引
	int m_nStartIdx;

	//	结束显示的K线索引
	int m_nEndIdx;

	//	选中的K线索引
	int m_nCurIdx;

	//	K线数据
	KLineCollection* m_pKLines;

	//	显示区域
	CRect m_Rect;

	//	是否显示成交量
	bool m_bShowVol;

	//	是否显示均价线
	bool m_bShowAvg;

	//	是否显示均线
	bool m_bShowMA;

	//	是否显示关键时间线
	bool m_bShowCriticalTime;

	//	K图与成交量图的高度比
	int m_nKVolRatio;

	//	K线之间的间距
	int m_nKSpace;

	//	本区域是否被选中
	bool m_bSelected;

	//	显示选中K线的特定价格
	int m_nSelectedPrice;

	//  鼠标所在的位置
	CPoint m_cp;

	//	每个价格单位对应的像素数
	float m_pixelPerPrice;

	//	当前图显示的最高价
	int m_kHighPrice;

	//	设置开盘价索引 -1为无
	int m_nOpenIndex;

	//	显示为日期(星期几)
	bool m_bShowDate;

public:

	//	放大K线图
	void ZoomIn();

	//	缩小K线图
	void ZoomOut();

	void MovePrev();

	void MoveNext();

public:
	
	KLineRenderer(void);
	~KLineRenderer(void);

	void AdjustIndex();

	void SetShowVol(bool bShow) { m_bShowVol = bShow; }
	void SetShowAvg(bool bShow) { m_bShowAvg = bShow; }
	void SetShowMA(bool bShow) { m_bShowMA = bShow; }
	void SetShowCriticalTime(bool bShow) { m_bShowCriticalTime = bShow; }
	void SetShowDate(bool bShow) { m_bShowDate = bShow; }

	void SetRenderMode(RenderMode mode) { m_enRenderMode = mode; }

	//	设置K线数据
	void SetKLineData(KLineCollection* pKLines);

	//	设置绘制范围
	void SetRect(CRect rect) { m_Rect = rect; }

	//	选中本区域并选中K线
	void Select(CPoint pt);

	//	绘制K线
	void Render(CDC* pDC);

	bool IsSelected() { return m_bSelected; }

	int GetCurTime();

	void SelectByTime(int nTime, bool bKeepScale = false);

	void ToggleRenderMode();

	void ToggleTrackingMode();

	void SetSelectedPrice(int price);

	int GetSelectedClosePrice();

	//	获取鼠标所在位置的价格
	int GetMousePrice(CPoint pt);

	TrackingMode GetTrackingMode() { return m_enTrackingMode; }

	void SetOpenIndex(int nOpenIndex) { m_nOpenIndex = nOpenIndex; }
};
