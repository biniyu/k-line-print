#pragma once

class KLineCollection;
class TradeRecordCollection;

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

	//	选中止损线
	bool m_bLossStopSelected;

	//	选中止盈线
	bool m_bProfitStopSelected;

	//	选中触发线
	bool m_bTriggerSelected;

//=================================================//

	//	视图显示的K线数量
	int m_nDisplayKLineCount;

	//	视图显示的首K线在K线数据中的索引
	int m_nFirstDisplayedIndex;

	//	选中的K线索引
	int m_nSelectedIndex;

	//	K线数据
	KLineCollection* m_pKLines;

	//	交易记录
	TradeRecordCollection* m_pTradeRecords;

	//	显示区域
	CRect m_Rect;

	//	是否显示分时线
	bool m_bShowTimeLine;

	//	是否显示成交量
	bool m_bShowVol;

	//	是否显示均价线
	bool m_bShowAvg;

	//	是否显示均线
	bool m_bShowMA;

	//	是否显示n日高低点
	bool m_bShowHighLow;

	//	是否显示日内最高最低价
	bool m_bShowMaxMin;

	//	是否显示关键时间线
	bool m_bShowCriticalTime;

	//	是否显示关键价格
	bool m_bShowKeyPrice;

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

	//	每个价格/成交量/持仓量单位对应的像素数
	float m_pixelPerPrice, m_pixelPerVol, m_pixelPerInterest;

	//	价格百分比
	float m_fPricePercentage;

	//	当前K线的左右位置
	float m_kLeft, m_kMiddle, m_kRight, m_kWidth;

	//	当前K线的上下位置
	float m_kHighPos, m_kLowPos, m_kOpenPos, m_kClosePos;

	//	最大成交量/持仓量
	int m_volMax, m_interestMax;

	//	当前图显示的最高/最低/中轴价
	int m_kHighPrice, m_kLowPrice, m_kAxisPrice;

	//	设置开盘价索引 -1为无
	int m_nOpenIndex;

	//	显示为日期(星期几)
	bool m_bShowDate;

private:

	CFont font;

	CPen penRed, penGreen, penBlue, penBlack, penGreyDotted, penOrange;
	CPen penRedDotted, penGreenDotted, penBlueDotted;
	CPen penRedBold, penGreenBold, penBlueBold;

	CBrush brRed, brGreen, brBlue;

private:

	void RenderGraphFrame(CDC* pDC);

	//	绘制持仓
	void RenderPosition(CDC* pDC);

	void RenderKeyPrice(CDC* pDC);
	void RenderMaxMinPrice(CDC* pDC);
	void RenderAxis(CDC* pDC);

	void RenderTradeRecord(CDC* pDC, int nKIdx);
	void RenderKLine(CDC* pDC, int nKIdx);
	void RenderTimeLine(CDC* pDC, int nKIdx);
	void RenderCriticalTime(CDC* pDC, int nKIdx);
	void RenderHighLowPrice(CDC* pDC, int nKIdx);
	void RenderSelection(CDC* pDC, int nKIdx);
	void RenderAvg(CDC* pDC, int nKIdx);
	void RenderMA(CDC* pDC, int nKIdx);
	void RenderVol(CDC* pDC, int nKIdx);


public:

	//	放大K线图
	void ZoomIn();

	//	缩小K线图
	void ZoomOut();

	void MovePrev();

	void MoveNext();

	//	删除触发器
	void DelTrigger();

public:
	
	KLineRenderer(void);
	~KLineRenderer(void);

	void AdjustIndex();

	void SetShowVol(bool bShow) { m_bShowVol = bShow; }
	void SetShowAvg(bool bShow) { m_bShowAvg = bShow; }
	void SetShowMA(bool bShow) { m_bShowMA = bShow; }
	void SetShowCriticalTime(bool bShow) { m_bShowCriticalTime = bShow; }
	void SetShowDate(bool bShow) { m_bShowDate = bShow; }
	void SetShowHighLow(bool bShow) { m_bShowHighLow = bShow; }
	void SetShowMaxMin(bool bShow) { m_bShowMaxMin = bShow; }

	void SetRenderMode(RenderMode mode) { m_enRenderMode = mode; }

	//	设置K线数据(包括默认显示多少K线位置)
	void SetKLineData(KLineCollection* pKLines, int nDispKLineCount = -1);

	//	设置交易记录数据
	void SetTradeRecord(TradeRecordCollection* pTrs) { m_pTradeRecords = pTrs; }

	//	设置绘制范围
	void SetRect(CRect rect) { m_Rect = rect; }

	//	选中本区域并选中K线
	void Select(CPoint pt);

	//	计算价格在图中的位置
	float GetPricePosition(int nPrice);

	//	计算成交量在图中的位置
	float GetVolPosition(int nVol);

	//	计算持仓量在图中的位置
	float GetInterestPosition(int nInterest);

	//	计算K线图的整体绘制位置
	void CalculateGraphLayout();

	//	计算K线的绘制位置
	void CalculateKLineLayout(int nKIdx);

	//	绘制K线
	void Render(CDC* pDC);

	bool IsSelected() { return m_bSelected; }

	int GetCurTime();

	void SelectByTime(int nTime);

	void SelectLastK();

	void ToggleRenderMode();

	void ToggleKeyPrice() { m_bShowKeyPrice = !m_bShowKeyPrice; }

	void ToggleTimeLine() { m_bShowTimeLine = !m_bShowTimeLine; }

	void SetLossStopPrice(int price);
	void SetProfitStopPrice(int price);
	void SetTriggerPrice(int price);

	void SetSelectedPrice(int price);

	int GetSelectedClosePrice();

	//	获取鼠标所在位置的价格
	int GetPriceByPosition(CPoint pt);

	void SetOpenIndex(int nOpenIndex) { m_nOpenIndex = nOpenIndex; }
};
