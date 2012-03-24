#pragma once

class KLineCollection;

class KLineRenderer
{
private:

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

	//	����K������
	void SetKLineData(KLineCollection* pKLines);

	//	���û��Ʒ�Χ
	void SetRect(CRect rect) { m_Rect = rect; }

	//	ѡ�б�����ѡ��K��
	void Select(CPoint pt);

	//	����K��
	void Render(CDC* pDC);
};
