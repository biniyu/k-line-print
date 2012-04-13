
// KLinePrintView.h : CKLinePrintView ��Ľӿ�
//
 
#pragma once

#include "KLineRenderer.h"

class KLineCollection;
class CKLinePrintDoc;

class CKLinePrintView : public CView
{
protected: // �������л�����
	CKLinePrintView();
	DECLARE_DYNCREATE(CKLinePrintView)

// ����
public:
	CKLinePrintDoc* GetDocument() const;

// ����
public:

	void Render();

	void Set5SecData(KLineCollection* pData) 
	{ 
		klr_5sec.SetKLineData(pData, 120); 
		klr_5sec.SetOpenIndex(0);
	}

	void Set1MinData(KLineCollection* pData) 
	{ 
		klr_1min.SetKLineData(pData, 240); 
		klr_1min.SetRenderMode(KLineRenderer::enAxisMode);
		klr_1min.SetShowAvg(true);
		klr_1min.SetShowCriticalTime(true);
		klr_1min.SetOpenIndex(1);
	}

	void SetDayData(KLineCollection* pData, int nDate)  
	{ 
		klr_day.SetKLineData(pData, 120);
		klr_day.SelectByTime(nDate);
		klr_day.SetShowVol(false);
		klr_day.SetShowMA(true);
		klr_day.SetOpenIndex(-1);
		klr_day.SetShowDate(true);
	}

private:

	bool m_bLocked;				//	����
	int	 m_nPlaybackSpeed;		//	��ӳ�ٶ�	 

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CKLinePrintView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	KLineRenderer klr_5sec;
	KLineRenderer klr_1min;
	KLineRenderer klr_day;

	CDC			m_MemDC;
	CBitmap		m_MemBitmap;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPlaybackBegin();
	afx_msg void OnPlaybackEnd();
	afx_msg void OnPlaybackForward();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPlaybackPause();
	afx_msg void OnPlaybackFastfw();
	afx_msg void OnPlaybackFastrev();
};

#ifndef _DEBUG  // KLinePrintView.cpp �еĵ��԰汾
inline CKLinePrintDoc* CKLinePrintView::GetDocument() const
   { return reinterpret_cast<CKLinePrintDoc*>(m_pDocument); }
#endif

