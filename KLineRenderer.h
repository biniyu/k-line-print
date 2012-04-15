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

//=================================================//

	//	��ͼ��ʾ��K������
	int m_nDisplayKLineCount;

	//	��ͼ��ʾ����K����K�������е�����
	int m_nFirstDisplayedIndex;

	//	ѡ�е�K������
	int m_nSelectedIndex;

	//	K������
	KLineCollection* m_pKLines;

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

	//	ÿ���۸�λ��Ӧ��������
	float m_pixelPerPrice;

	//	��ǰͼ��ʾ����߼�
	int m_kHighPrice;

	//	���ÿ��̼����� -1Ϊ��
	int m_nOpenIndex;

	//	��ʾΪ����(���ڼ�)
	bool m_bShowDate;

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

	//	���û��Ʒ�Χ
	void SetRect(CRect rect) { m_Rect = rect; }

	//	ѡ�б�����ѡ��K��
	void Select(CPoint pt);

	//	����K��
	void Render(CDC* pDC);

	bool IsSelected() { return m_bSelected; }

	int GetCurTime();

	void SelectByTime(int nTime);

	void SelectLastK();

	void ToggleRenderMode();

	void ToggleTrackingMode();

	void SetSelectedPrice(int price);

	int GetSelectedClosePrice();

	//	��ȡ�������λ�õļ۸�
	int GetMousePrice(CPoint pt);

	TrackingMode GetTrackingMode() { return m_enTrackingMode; }

	void SetOpenIndex(int nOpenIndex) { m_nOpenIndex = nOpenIndex; }
};
