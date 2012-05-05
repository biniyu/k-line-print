
// KLinePrintView.h : CKLinePrintView 类的接口
//
 
#pragma once

#include "KLineRenderer.h"
#include "TickRenderer.h"
#include "TradeDialog.h"

class KLineCollection;
class CKLinePrintDoc;

class CKLinePrintView : public CView
{
protected: // 仅从序列化创建
	CKLinePrintView();
	DECLARE_DYNCREATE(CKLinePrintView)

// 属性
public:
	CKLinePrintDoc* GetDocument() const;

// 操作
public:

	void Layout();
	void Render();

	void SetTickData(Tick tick);
	void Set5SecData(KLineCollection* pData);
	void Set1MinData(KLineCollection* pData);
	void SetDayData(KLineCollection* pData, int nDate);  

private:

	enum ViewMode
	{
		ViewModeAll,
		ViewModeDay,
		ViewMode1Min,
		ViewModeDetail
	};

	bool		m_bLocked;				//	联动
	int			m_nPlaybackSpeed;		//	放映速度
	ViewMode	m_enViewMode;

	void ToggleViewMode();		//	切换视图

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
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
	TickRenderer  tick_render;

	CDC			m_MemDC;
	CBitmap		m_MemBitmap;

	CTradeDialog* m_pTradeDialog;

// 生成的消息映射函数
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
	afx_msg void OnPlaybackStop();
	afx_msg void OnPlaybackConf();
};

#ifndef _DEBUG  // KLinePrintView.cpp 中的调试版本
inline CKLinePrintDoc* CKLinePrintView::GetDocument() const
   { return reinterpret_cast<CKLinePrintDoc*>(m_pDocument); }
#endif

