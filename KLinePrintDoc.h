
// KLinePrintDoc.h : CKLinePrintDoc ��Ľӿ�
//
#include <string>
#include "KLineCollection.h"
#include "TickReader.h"

using namespace std;

#pragma once

class CKLinePrintDoc : public CDocument
{
protected: // �������л�����
	CKLinePrintDoc();
	DECLARE_DYNCREATE(CKLinePrintDoc)

private:

	int	m_nCurrentTickIdx;		//	��һ��������tick������

// ����
public:

	TickCollection tc;
	KLineCollection klc15s;
	KLineCollection klc1min;
	KLineCollection klcday;

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
	string m_CurDayFile;

	void ViewNeighborDate(BOOL bPrev);
	void ReloadByDate(int nDate);
	void LoadKLineGroup(string targetCsvFile);
	void ReloadDetailData(int second);

	/* ��ʾ��ĳ��ʱ�� (-1�ǲ������У� 0�ǲ�����tick�������ǲ�����Ŀ��ʱ��)*/
	void DisplayTillTime(int nTillTime = -1);

	/* ������ĳ��ʱ�� */
	BOOL PlayTillTime(int nTillTime);

	// ��ȡ��ǰʱ��	
	int GetCurrentTickTime();

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();
};


