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

	//	��ʼ��ʾ��K������
	int m_nStartIdx;

	//	������ʾ��K������
	int m_nEndIdx;

	//	ѡ�е�K������
	int m_nCurIdx;

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

	void SetRenderMode(RenderMode mode) { m_enRenderMode = mode; }

	//	����K������
	void SetKLineData(KLineCollection* pKLines);

	//	���û��Ʒ�Χ
	void SetRect(CRect rect) { m_Rect = rect; }

	//	ѡ�б�����ѡ��K��
	void Select(CPoint pt);

	//	����K��
	void Render(CDC* pDC);

	bool IsSelected() { return m_bSelected; }

	int GetCurTime();

	void SelectByTime(int nTime, bool bKeepScale = false);

	void ToggleRenderMode();

	void ToggleTrackingMode();

	void SetSelectedPrice(int price);

	int GetSelectedClosePrice();

	//	��ȡ�������λ�õļ۸�
	int GetMousePrice(CPoint pt);

	TrackingMode GetTrackingMode() { return m_enTrackingMode; }

	void SetOpenIndex(int nOpenIndex) { m_nOpenIndex = nOpenIndex; }
};
