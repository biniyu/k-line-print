
// KLinePrintDoc.h : CKLinePrintDoc ��Ľӿ�
//
#include <string>
#include "KLineCollection.h"

using namespace std;

#pragma once

class CKLinePrintDoc : public CDocument
{
protected: // �������л�����
	CKLinePrintDoc();
	DECLARE_DYNCREATE(CKLinePrintDoc)

// ����
public:

	KLineCollection klc15s;
	KLineCollection klc1min;

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CKLinePrintDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	string m_CurCsvFile;
	string GetNeighborCsvFile(string path, bool bPrev, bool bZhuLi = FALSE);
	void ViewNeighborDate(BOOL bPrev);

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();
};


