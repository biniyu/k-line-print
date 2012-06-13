#include "StdAfx.h"
#include "KLinePrint.h"
#include "Utility.h"
#include "direct.h"
#include "io.h"
#include <fstream>

using namespace std;

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

string CStringTostring(const CString& str)
{
	char InfoString[256];    
	  
	// 转换后的数据存放在InfoString数组中   
	if (!WideCharToMultiByte(CP_ACP,0,LPCTSTR(str),-1,InfoString,100,NULL,NULL))    
	{    
		return "";    
	} 

	return string(InfoString);
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

string Utility::GetProgramPath()
{
	char szPath[MAX_PATH];   
	GetModuleFileNameA(NULL, szPath, MAX_PATH);
	string PathName(szPath);
	size_t npos = PathName.find_last_of('\\');
	return PathName.substr(0, npos + 1);
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

vector<string> Utility::GetAllContractPath(string path)
{
	int date;
	char buf[512];

	vector<string> contracts;

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

	for(int i = 0; i < vecFiles.size(); i++)
	{
		// 过滤PI/MI/VI文件

		if((vecFiles[i].find("PI_") != string::npos)
		|| (vecFiles[i].find("MI_") != string::npos)
		|| (vecFiles[i].find("VI_") != string::npos)) continue;

		contracts.push_back(dir + vecFiles[i]);
	}

	return contracts;
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

	int weekday =  (y + y/4 + c/4 - 2*c + 26*(m+1)/10 + d-1) % 7;

	if(weekday < 0)
	{
		weekday += 7;
	}

	return weekday;
}

#define CONFIG_FILE ((Utility::GetProgramPath() + "klinep.ini").c_str())

void WritePrivateProfileIntA(LPCSTR lpAppName, LPCSTR lpKeyName, INT nDefault, LPCSTR lpFileName)
{
	char buf[64];
	sprintf(buf, "%d", nDefault);
	WritePrivateProfileStringA(lpAppName, lpKeyName, buf, lpFileName);
}

//	读入账户/品种信息
void Utility::ReadExchangeConfig(int& nBalance, int& nFee, int& nMargin, int& nUnitsPerSlot)
{
	nBalance = GetPrivateProfileIntA("Exchange","Balance", 50000, CONFIG_FILE);
	nFee = GetPrivateProfileIntA("Exchange","Fee", 10, CONFIG_FILE);
	nMargin = GetPrivateProfileIntA("Exchange","Margin", 12, CONFIG_FILE);
	nUnitsPerSlot = GetPrivateProfileIntA("Exchange","UnitsPerSlot", 5, CONFIG_FILE);
}

void Utility::WriteBalance(int nBalance)
{
	WritePrivateProfileIntA("Exchange","Balance", nBalance, CONFIG_FILE);
}

void Utility::WriteExchangeConfig(int nFee, int nMargin, int nUnitsPerSlot)
{
	WritePrivateProfileIntA("Exchange","Fee", nFee, CONFIG_FILE);
	WritePrivateProfileIntA("Exchange","Margin", nMargin, CONFIG_FILE);
	WritePrivateProfileIntA("Exchange","UnitsPerSlot", nUnitsPerSlot, CONFIG_FILE);
}

//	读入回放配置
PlaybackConfig Utility::ReadPlaybackConfig()
{
	PlaybackConfig pc;

	pc.enPlaybackOrder = (PlaybackConfig::PlaybackOrder)
		GetPrivateProfileIntA("Playback","Order", 0, CONFIG_FILE);
	
	pc.nStartDate = GetPrivateProfileIntA("Playback","StartDate", 0, CONFIG_FILE);
	pc.nEndDate = GetPrivateProfileIntA("Playback","EndDate", 0, CONFIG_FILE);
	pc.nStartTime = GetPrivateProfileIntA("Playback","StartTime", 0, CONFIG_FILE);
	pc.nEndTime = GetPrivateProfileIntA("Playback","EndTime", 0, CONFIG_FILE);

	pc.bDayOfWeek[1] = GetPrivateProfileIntA("Playback","Monday", 0, CONFIG_FILE);
	pc.bDayOfWeek[2] = GetPrivateProfileIntA("Playback","Tuesday", 0, CONFIG_FILE);
	pc.bDayOfWeek[3] = GetPrivateProfileIntA("Playback","Wednesday", 0, CONFIG_FILE);
	pc.bDayOfWeek[4] = GetPrivateProfileIntA("Playback","Thursday", 0, CONFIG_FILE);
	pc.bDayOfWeek[5] = GetPrivateProfileIntA("Playback","Friday", 0, CONFIG_FILE);

	pc.fGapPercentage = GetPrivateProfileIntA("Playback","Gap", 0, CONFIG_FILE);
	pc.fLastDayFluctuationAbove = GetPrivateProfileIntA("Playback","FluctuationAbove", 0, CONFIG_FILE);
	pc.fLastDayFluctuationBelow = GetPrivateProfileIntA("Playback","FluctuationBelow", 0, CONFIG_FILE);

	pc.bRealTime = GetPrivateProfileIntA("Playback","RealTime", 1, CONFIG_FILE);
	pc.nPlaySpeed = GetPrivateProfileIntA("Playback","Speed", 1, CONFIG_FILE);

	return pc;
}

//	保存回放配置
void Utility::SavePlaybackConfig(PlaybackConfig pc)
{
	WritePrivateProfileIntA("Playback","RealTime", pc.bRealTime, CONFIG_FILE);
	WritePrivateProfileIntA("Playback","Speed", pc.nPlaySpeed, CONFIG_FILE);

	WritePrivateProfileIntA("Playback","Order", (int)pc.enPlaybackOrder, CONFIG_FILE);

	WritePrivateProfileIntA("Playback","StartDate", pc.nStartDate, CONFIG_FILE);
	WritePrivateProfileIntA("Playback","EndDate", pc.nEndDate, CONFIG_FILE);
	WritePrivateProfileIntA("Playback","StartTime", pc.nStartTime, CONFIG_FILE);
	WritePrivateProfileIntA("Playback","EndTime", pc.nEndTime, CONFIG_FILE);

	WritePrivateProfileIntA("Playback","Monday", (int)pc.bDayOfWeek[1], CONFIG_FILE);
	WritePrivateProfileIntA("Playback","Tuesday", (int)pc.bDayOfWeek[2], CONFIG_FILE);
	WritePrivateProfileIntA("Playback","Wednesday", (int)pc.bDayOfWeek[3], CONFIG_FILE);
	WritePrivateProfileIntA("Playback","Thursday", (int)pc.bDayOfWeek[4], CONFIG_FILE);
	WritePrivateProfileIntA("Playback","Friday", (int)pc.bDayOfWeek[5], CONFIG_FILE);

	WritePrivateProfileIntA("Playback","Gap", (int)pc.fGapPercentage, CONFIG_FILE);
	WritePrivateProfileIntA("Playback","FluctuationAbove", (int)pc.fLastDayFluctuationAbove, CONFIG_FILE);
	WritePrivateProfileIntA("Playback","FluctuationBelow", (int)pc.fLastDayFluctuationBelow, CONFIG_FILE);
}

void Utility::WriteLog(TradeRecord tr)
{
	ofstream outfile;
	string logfile = GetProgramPath() + "tradelog.txt";

	outfile.open(logfile.c_str(),ios::app);
	
	outfile << tr.nRealDate << " " << tr.nRealTime << " " << tr.sFileName << " " 
		<< tr.nSimuTime << " " << tr.nPrice << " " 
		<< tr.bBuy << " " << tr.bOpen << " " << tr.nSlot << " "
		<< tr.nFee << " " << tr.nProfit << endl;

	outfile.close();
}

void Utility::ReadLog(vector<TradeRecord>& trs)
{
	ifstream in;
	TradeRecord tr;

	string logfile = GetProgramPath() + "tradelog.txt";

	in.open(logfile.c_str());

	while(!in.eof())
	{
		in >> tr.nRealDate >> tr.nRealTime >> tr.sFileName
			>> tr.nSimuTime >> tr.nPrice >> tr.bBuy >> tr.bOpen
			>> tr.nSlot >> tr.nFee >> tr.nProfit;

		trs.push_back(tr);
	}
}


