#include "StdAfx.h"
#include "KLinePrint.h"
#include "Utility.h"
#include "direct.h"
#include "io.h"

Utility::Utility(void)
{
}

Utility::~Utility(void)
{
}

int CStringToInt(const CString& str)
{
	char InfoString[256];    
	  
	// 转换后的数据存放在InfoString数组中   
	if (!WideCharToMultiByte(CP_ACP,0,LPCTSTR(str),-1,InfoString,100,NULL,NULL))    
	{    
		return -1;    
	} 

	int ip = atoi(InfoString);
	
	return ip;
}

CString IntToCString(int nInt)
{
	CString str;

	str.Format(_T("%d"), nInt);

	return str;
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

vector<string> GetFiles(string dir, string filter, bool bDir)
{
	vector<string> vecFiles;
	
	if(_chdir(dir.c_str())) return vecFiles;
	else
	{
		long hFile;
		_finddata_t fileinfo;
		
		if((hFile = _findfirst(filter.c_str(), &fileinfo))!= -1)
		{
			do
			{
				if(!strcmp(".", fileinfo.name)) continue;
				if(!strcmp("..", fileinfo.name)) continue;
				
				if((bDir && (fileinfo.attrib & _A_SUBDIR))
					|| (!bDir && !(fileinfo.attrib & _A_SUBDIR)))
				{
					vecFiles.push_back(string(fileinfo.name));
				}
				
			} while(_findnext(hFile, &fileinfo) == 0);
			
			_findclose(hFile);
		}
	}
	
	return vecFiles;
}

void Utility::GetInfoByPath(string path, string& rootdir, 
								string& market, string& contract, int& date)
{

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

	rootdir = sub3.substr(0, posBackSlash);

	size_t posUnderScore = filename.find_last_of('_');

	string dateTmp = filename.substr(posUnderScore + 1);

	date = atoi(dateTmp.c_str());
	contract = filename.substr(0, posUnderScore);
}

string Utility::GetMajorContractPath(string path)
{
	int date;
	char buf[512];
	string rootdir, contract, var, market, final;

	GetInfoByPath(path, rootdir, market, contract, date);

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
		rootdir.c_str(),
		market.c_str(),
		market.c_str(),
		date/100,
		date);

	string dir = buf;

	sprintf(buf, "%s*_%d.csv",
		var.c_str(),
		date);

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

	if(nMaxFileIndex == -1) return "";
	else return dir + vecFiles[nMaxFileIndex];
}

string Utility::GetDayLinePath(string path)
{
	int date;
	char buf[512];
	string rootdir, contract, market;

	GetInfoByPath(path, rootdir, market, contract, date);

	sprintf(buf, "%s\\DAY\\%s%s.TXT", 
		rootdir.c_str(),
		market.c_str(),
		contract.c_str());

	return string(buf);
}

int Utility::GetDateByPath(string path)
{
	int date;
	string rootdir, contract, market;

	GetInfoByPath(path, rootdir, market, contract, date);
	return date;
}

string Utility::GetPathByDate(string org_path, int date)
{
	char buf[512];
	int tmp;
	string rootdir, contract, market;

	GetInfoByPath(org_path, rootdir, market, contract, tmp);

	sprintf(buf, "%s\\%s\\%s%d\\%d\\%s_%d.csv",
		rootdir.c_str(),
		market.c_str(),
		market.c_str(),
		date/100,
		date,
		contract.c_str(),
		date);

	return string(buf);
}

string Utility::GetNeighborCsvFile(string path, bool bPrev, bool bZhuLi)
{
	int date;
	char buf[512];
	string rootdir, contract, market;

	Utility::GetInfoByPath(path, rootdir, market, contract, date);

	int nNeighDate;

	if(bPrev)
		nNeighDate = CALENDAR.GetPrev(date);
	else
		nNeighDate = CALENDAR.GetNext(date); 

	if(nNeighDate < 0) return "";

	sprintf(buf, "%s\\%s\\%s%d\\%d\\%s_%d.csv", 
		rootdir.c_str(),
		market.c_str(),
		market.c_str(),
		nNeighDate/100,
		nNeighDate,
		contract.c_str(),
		nNeighDate);

	if(bZhuLi) /* 搜索主力合约 */
	{
		return Utility::GetMajorContractPath(buf);
	}
	else
	{
		return string(buf);
	}
}

int Utility::ConvDispTimeToContTime(int disp)
{
	int tmphour, tmpmin, tmpsec;

	tmphour = disp / 10000;
	tmpmin = disp % 10000 / 100;
	tmpsec = disp % 10000 % 100;

	return tmphour * 3600 + tmpmin * 60 + tmpsec;
}

int Utility::ConvContTimeToDispTime(int cont)
{
	int tmphour, tmpmin, tmpsec;

	tmphour = cont / 3600;
	tmpmin = cont % 3600 / 60;
	tmpsec = cont % 3600 % 60;

	return tmphour * 10000 + tmpmin * 100 + tmpsec;
	
}

int Utility::GetWeekDayByDate(int nDate)
{
	// w=y+[y/4]+[c/4]-2c+[26(m+1)/10]+d-1

	int c = nDate / 1000000;
	int y = nDate % 1000000 / 10000;
	int m = nDate % 1000000 % 10000 / 100;
	int d = nDate % 1000000 % 10000 % 100;

	if(m <= 2)
	{
		m += 12;
		y -= 1;
	}

	return (y + y/4 + c/4 - 2*c + 26*(m+1)/10 + d-1) % 7;
}


