
// KLinePrintDoc.cpp : CKLinePrintDoc 类的实现
//

#include "stdafx.h"
#include "KLinePrint.h"

#include "KLinePrintDoc.h"
#include "TickReader.h"
#include "KLineCollection.h"
#include "CalendarGenerator.h"
#include "direct.h"
#include "io.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKLinePrintDoc

IMPLEMENT_DYNCREATE(CKLinePrintDoc, CDocument)

BEGIN_MESSAGE_MAP(CKLinePrintDoc, CDocument)
	ON_COMMAND(ID_FILE_OPEN, &CKLinePrintDoc::OnFileOpen)
END_MESSAGE_MAP()


// CKLinePrintDoc 构造/析构

CKLinePrintDoc::CKLinePrintDoc()
{
	// TODO: 在此添加一次性构造代码

}

CKLinePrintDoc::~CKLinePrintDoc()
{
}

BOOL CKLinePrintDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CKLinePrintDoc 序列化

void CKLinePrintDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CKLinePrintDoc 诊断

#ifdef _DEBUG
void CKLinePrintDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CKLinePrintDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CKLinePrintDoc 命令

void CKLinePrintDoc::OnFileOpen()
{
	CFileDialog dlg(TRUE); ///TRUE为OPEN对话框，FALSE为SAVE AS对话框
	if(dlg.DoModal() == IDOK)
	{
		CString FilePathName = dlg.GetPathName(); 

		klc.clear();

		TickReader tr;

		TickCollection tcPrev;

		TickCollection tc;

		char InfoString[256];    
		  
		// 转换后的数据存放在InfoString数组中   
		if (!WideCharToMultiByte(CP_ACP,0,LPCTSTR(FilePathName),-1,InfoString,100,NULL,NULL))    
		{    
			return;    
		} 

		m_CurCsvFile = InfoString;

		tr.Read(InfoString, tc);

		tr.Read(GetNeighborCsvFile(InfoString, TRUE, FALSE), tcPrev);

		KLine kline;

		kline.close = tcPrev.close;
		kline.open = tcPrev.close;
		kline.high = tcPrev.high;
		kline.low = tcPrev.low;
		kline.vol = tcPrev.avgvol;
	
		klc.Generate(tc, 30, kline);

		this->SetTitle(CString(m_CurCsvFile.c_str()));
		
		this->UpdateAllViews(0);
	}
}

int GetFileSize(string dir, string filename)
{
	vector<string> vecFiles;
	
	if(_chdir(dir.c_str())) return 0;
	else
	{
		long hFile;
		_finddata_t fileinfo;
		
		if((hFile = _findfirst(filename.c_str(), &fileinfo))!= -1)
		{
			_findclose(hFile);
			return fileinfo.size;			
		}

		return 0;
	}
}

string CKLinePrintDoc::GetNeighborCsvFile(string path, bool bPrev, bool bZhuLi)
{
	//	从文件名获取品种，合约与日期
	int date;
	char buf[512];
	string contract, var, market, final;

	size_t posDot = path.find_last_of('.');
	size_t posBackSlash = path.find_last_of('\\');

	string filename = path.substr(posBackSlash+1, posDot - posBackSlash - 1);

	string sub1 = path.substr(0, posBackSlash);
	posBackSlash = sub1.find_last_of('\\');

	string sub2 = sub1.substr(0, posBackSlash);
	posBackSlash = sub2.find_last_of('\\');
	
	string sub3 = sub2.substr(0, posBackSlash);
	posBackSlash = sub3.find_last_of('\\');

	market = sub3.substr(sub3.size()-2, 2);

	string rootDir = sub3.substr(0, posBackSlash);

	size_t posUnderScore = filename.find_last_of('_');

	string dateTmp = filename.substr(posUnderScore + 1);

	date = atoi(dateTmp.c_str());
	contract = filename.substr(0, posUnderScore);

	int nNeighDate;

	if(bPrev)
		nNeighDate = ((CKLinePrintApp*)AfxGetApp())->cal.GetPrev(date);
	else
		nNeighDate = ((CKLinePrintApp*)AfxGetApp())->cal.GetNext(date); 

	if(bZhuLi) /* 搜索主力合约 */
	{
		//	合约最后两位是月份
		var = contract.substr(0, contract.size() - 2);

		//	处理跨年合约
		if(var[var.size() - 1] == 'X'
			|| var[var.size() - 1] == 'Y')
		{
			var = var.substr(0, var.size() - 1);
		}

		// 在该目录搜索最大的文件(需要过滤掉MI,PI,VI文件)
		vector<string> vecFiles;

		sprintf(buf, "%s\\%s\\%s%d\\%d\\",
			rootDir.c_str(),
			market.c_str(),
			market.c_str(),
			nNeighDate/100,
			nNeighDate);

		string dir = buf;

		sprintf(buf, "%s*_%d.csv",
			var.c_str(),
			nNeighDate);

		vecFiles = GetFiles(dir, buf, false);

		int nMaxFileSize = 0;
		int nMaxFileIndex = -1;

		for(int i = 0; i < vecFiles.size(); i++)
		{
			// 过滤PI/MI/VI文件

			if((vecFiles[i].find("PI_") != string::npos)
			|| (vecFiles[i].find("MI_") != string::npos)
			|| (vecFiles[i].find("VI_") != string::npos)) continue;

			int nFileSize = GetFileSize(dir, vecFiles[i]);

			if(nFileSize > nMaxFileSize)
			{
				nMaxFileSize = nFileSize;
				nMaxFileIndex = i;
			}	
		}

		return dir + vecFiles[nMaxFileIndex];
	}

	sprintf(buf, "%s\\%s\\%s%d\\%d\\%s_%d.csv", 
		rootDir.c_str(),
		market.c_str(),
		market.c_str(),
		nNeighDate/100,
		nNeighDate,
		contract.c_str(),
		nNeighDate);

	return string(buf);

}

void CKLinePrintDoc::ViewNeighborDate(BOOL bPrev)
{
	klc.clear();

	TickReader tr;

	TickCollection tcPrev;

	TickCollection tc;

	m_CurCsvFile = GetNeighborCsvFile(m_CurCsvFile, bPrev, TRUE/* 必须是主力合约 */);

	tr.Read(m_CurCsvFile, tc);

	tr.Read(GetNeighborCsvFile(m_CurCsvFile, TRUE, FALSE/* 必须是相同合约，保持连续 */), tcPrev);

	KLine kline;

	kline.close = tcPrev.close;
	kline.open = tcPrev.close;
	kline.high = tcPrev.high;
	kline.low = tcPrev.low;
	kline.vol = tcPrev.avgvol;

	klc.Generate(tc, 30, kline);

	this->SetTitle(CString(m_CurCsvFile.c_str()));
	
	this->UpdateAllViews(0);	
}
