
// KLinePrintDoc.h : CKLinePrintDoc 类的接口
//
#include <string>
#include "KLineCollection.h"
#include "TickReader.h"

using namespace std;

#pragma once

class CKLinePrintDoc : public CDocument
{
protected: // 仅从序列化创建
	CKLinePrintDoc();
	DECLARE_DYNCREATE(CKLinePrintDoc)

private:

	int	m_nCurrentTickIdx;		//	下一个待播放tick的索引

// 属性
public:

	TickCollection tc;
	KLineCollection klc15s;
	KLineCollection klc1min;
	KLineCollection klcday;

// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 实现
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

	/* 显示至某个时点 (-1是播放所有， 0是播放首tick，其他是播放至目标时间)*/
	void DisplayTillTime(int nTillTime = -1);

	/* 播放至某个时点 */
	BOOL PlayTillTime(int nTillTime);

	// 获取当前时点	
	int GetCurrentTickTime();

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();
};


