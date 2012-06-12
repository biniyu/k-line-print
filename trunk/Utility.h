#pragma once

#include <string>
#include <vector>
#include "PlaybackConfig.h"
#include "TradeFacility.h"

using namespace std;

int GetFileSize(string dir, string filename);
vector<string> GetFiles(string dir, string filter, bool bDir);

int CStringToInt(const CString& str);
CString IntToCString(int nInt);

class Utility
{
public:
	Utility(void);
	~Utility(void);

	//	读入回放配置
	static PlaybackConfig ReadPlaybackConfig();
	static void ReadExchangeConfig(int& nBalance, int& nFee, int& nMargin, int& nUnitsPerSlot);
	static void WriteBalance(int nBalance);
	static void WriteExchangeConfig(int nFee, int nMargin, int nUnitsPerSlot);

	//	保存回放配置
	static void SavePlaybackConfig(PlaybackConfig pc);

	static void GetInfoByPath(string path, string& rootdir, 
					string& market, string& contract, int& date);

	static vector<string> GetAllContractPath(string path);
	static string GetMajorContractPath(string path);
	static string GetDayLinePath(string path);
	static int GetDateByPath(string path);
	static string GetPathByDate(string org_path, int date);
	static string GetNeighborCsvFile(string path, bool bPrev, bool bZhuLi);

	//	连续时间与显示时间的转换
	static int ConvDispTimeToContTime(int disp);
	static int ConvContTimeToDispTime(int cont);

	static int GetWeekDayByDate(int nDate);
	static string GetProgramPath();
	static void WriteLog(TradeRecord tr);
};
