
// KLinePrintView.h : CKLinePrintView ��Ľӿ�
//

#pragma once


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

	CPoint cp;
	BOOL m_bDrawTrackingCrossLine;

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
};

#ifndef _DEBUG  // KLinePrintView.cpp �еĵ��԰汾
inline CKLinePrintDoc* CKLinePrintView::GetDocument() const
   { return reinterpret_cast<CKLinePrintDoc*>(m_pDocument); }
#endif

