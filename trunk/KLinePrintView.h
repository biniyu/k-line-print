
// KLinePrintView.h : CKLinePrintView 类的接口
//

#pragma once


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

	CPoint cp;
	BOOL m_bDrawTrackingCrossLine;

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
};

#ifndef _DEBUG  // KLinePrintView.cpp 中的调试版本
inline CKLinePrintDoc* CKLinePrintView::GetDocument() const
   { return reinterpret_cast<CKLinePrintDoc*>(m_pDocument); }
#endif

