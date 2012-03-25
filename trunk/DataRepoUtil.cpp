#include "StdAfx.h"
#include "DataRepoUtil.h"
#include "direct.h"
#include "io.h"

DataRepoUtil::DataRepoUtil(void)
{
}

DataRepoUtil::~DataRepoUtil(void)
{
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

void DataRepoUtil::GetInfoByPath(string path, string& rootdir, 
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

string DataRepoUtil::GetMajorContractPath(string path)
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

	return dir + vecFiles[nMaxFileIndex];
}

string DataRepoUtil::GetDayLinePath(string path)
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

int DataRepoUtil::GetDateByPath(string path)
{
	int date;
	string rootdir, contract, market;

	GetInfoByPath(path, rootdir, market, contract, date);
	return date;
}

string DataRepoUtil::GetPathByDate(string org_path, int date)
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
