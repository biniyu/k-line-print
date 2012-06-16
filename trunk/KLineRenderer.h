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

	enum TrackingMode
	{
		enHighLowTMode,
		enCloseTMode,
		enMouseTMode
	};

private:

	RenderMode m_enRenderMode;

	TrackingMode m_enTrackingMode;

//=================================================//

	//	��ͼ��ʾ��K������
	int m_nDisplayKLineCount;

	//	��ͼ��ʾ����K����K�������е�����
	int m_nFirstDisplayedIndex;

	//	ѡ�е�K������
	int m_nSelectedIndex;

	//	K������
	KLineCollection* m_pKLines;

	//	���׼�¼
	TradeRecordCollection* m_pTradeRecords;

	//	��ʾ����
	CRect m_Rect;

	//	�Ƿ���ʾ�ɽ���
	bool m_bShowVol;

	//	�Ƿ���ʾ������
	bool m_bShowAvg;

	//	�Ƿ���ʾ����
	bool m_bShowMA;

	//	�Ƿ���ʾn�ոߵ͵�
	bool m_bShowHighLow;

	//	�Ƿ���ʾ���������ͼ�
	bool m_bShowMaxMin;

	//	�Ƿ���ʾ�ؼ�ʱ����
	bool m_bShowCriticalTime;

	//	�Ƿ���ʾ�ؼ��۸�
	bool m_bShowKeyPrice;

	//	Kͼ��ɽ���ͼ�ĸ߶ȱ�
	int m_nKVolRatio;

	//	K��֮��ļ��
	int m_nKSpace;

	//	�������Ƿ�ѡ��
	bool m_bSelected;

	//	��ʾѡ��K�ߵ��ض��۸�
	int m_nSelectedPrice;

	//  ������ڵ�λ��
	CPoint m_cp;

	//	ÿ���۸�/�ɽ���/�ֲ�����λ��Ӧ��������
	float m_pixelPerPrice, m_pixelPerVol, m_pixelPerInterest;

	//	�۸�ٷֱ�
	float m_fPricePercentage;

	//	��ǰK�ߵ�����λ��
	float m_kLeft, m_kMiddle, m_kRight, m_kWidth;

	//	��ǰK�ߵ�����λ��
	float m_kHighPos, m_kLowPos, m_kOpenPos, m_kClosePos;

	//	���ɽ���/�ֲ���
	int m_volMax, m_interestMax;

	//	��ǰͼ��ʾ�����/���/�����
	int m_kHighPrice, m_kLowPrice, m_kAxisPrice;

	//	���ÿ��̼����� -1Ϊ��
	int m_nOpenIndex;

	//	��ʾΪ����(���ڼ�)
	bool m_bShowDate;

private:

	CFont font;

	CPen penRed, penGreen, penBlue, penBlack, penGreyDotted;
	CPen penRedDotted, penGreenDotted, penBlueDotted;

	CBrush brRed, brGreen, brBlue;

private:

	void RenderGraphFrame(CDC* pDC);

	void RenderKeyPrice(CDC* pDC);
	void RenderMaxMinPrice(CDC* pDC);
	void RenderAxis(CDC* pDC);

	void RenderTradeRecord(CDC* pDC, int nKIdx);
	void RenderKLine(CDC* pDC, int nKIdx);
	void RenderCriticalTime(CDC* pDC, int nKIdx);
	void RenderHighLowPrice(CDC* pDC, int nKIdx);
	void RenderSelection(CDC* pDC, int nKIdx);
	void RenderAvg(CDC* pDC, int nKIdx);
	void RenderMA(CDC* pDC, int nKIdx);
	void RenderVol(CDC* pDC, int nKIdx);

public:

	//	�Ŵ�K��ͼ
	void ZoomIn();

	//	��СK��ͼ
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
	void SetShowHighLow(bool bShow) { m_bShowHighLow = bShow; }
	void SetShowMaxMin(bool bShow) { m_bShowMaxMin = bShow; }

	void SetRenderMode(RenderMode mode) { m_enRenderMode = mode; }

	//	����K������(����Ĭ����ʾ����K��λ��)
	void SetKLineData(KLineCollection* pKLines, int nDispKLineCount = -1);

	//	���ý��׼�¼����
	void SetTradeRecord(TradeRecordCollection* pTrs) { m_pTradeRecords = pTrs; }

	//	���û��Ʒ�Χ
	void SetRect(CRect rect) { m_Rect = rect; }

	//	ѡ�б�����ѡ��K��
	void Select(CPoint pt);

	//	����۸���ͼ�е�λ��
	float GetPricePosition(int nPrice);

	//	����ɽ�����ͼ�е�λ��
	float GetVolPosition(int nVol);

	//	����ֲ�����ͼ�е�λ��
	float GetInterestPosition(int nInterest);

	//	����K��ͼ���������λ��
	void CalculateGraphLayout();

	//	����K�ߵĻ���λ��
	void CalculateKLineLayout(int nKIdx);

	//	����K��
	void Render(CDC* pDC);

	bool IsSelected() { return m_bSelected; }

	int GetCurTime();

	void SelectByTime(int nTime);

	void SelectLastK();

	void ToggleRenderMode();

	void ToggleTrackingMode();

	void ToggleKeyPrice() { m_bShowKeyPrice = !m_bShowKeyPrice; }

	void SetSelectedPrice(int price);

	int GetSelectedClosePrice();

	//	��ȡ�������λ�õļ۸�
	int GetMousePrice(CPoint pt);

	TrackingMode GetTrackingMode() { return m_enTrackingMode; }

	void SetOpenIndex(int nOpenIndex) { m_nOpenIndex = nOpenIndex; }
};
