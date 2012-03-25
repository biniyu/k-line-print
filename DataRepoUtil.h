#pragma once

#include <string>
#include <vector>

using namespace std;

int GetFileSize(string dir, string filename);
vector<string> GetFiles(string dir, string filter, bool bDir);

class DataRepoUtil
{
public:
	DataRepoUtil(void);
	~DataRepoUtil(void);

	static void GetInfoByPath(string path, string& rootdir, 
					string& market, string& contract, int& date);

	static string GetMajorContractPath(string path);
	static string GetDayLinePath(string path);
	static int GetDateByPath(string path);
	static string GetPathByDate(string org_path, int date);
};
