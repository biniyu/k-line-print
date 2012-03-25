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

private:

	RenderMode m_enRenderMode;

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

	//	K图与成交量图的高度比
	int m_nKVolRatio;

	//	K线之间的间距
	int m_nKSpace;

	//	本区域是否被选中
	bool m_bSelected;

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

	void SelectByTime(int nTime);

	void SwitchMode();
};
