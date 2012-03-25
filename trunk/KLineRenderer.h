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

	//	Kͼ��ɽ���ͼ�ĸ߶ȱ�
	int m_nKVolRatio;

	//	K��֮��ļ��
	int m_nKSpace;

	//	�������Ƿ�ѡ��
	bool m_bSelected;

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

	void SelectByTime(int nTime);

	void SwitchMode();
};
