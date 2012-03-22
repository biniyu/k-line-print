#include "StdAfx.h"
#include "CalendarGenerator.h"
#include <vector>
#include <string>
#include "direct.h"
#include "io.h"

using namespace std;

CalendarGenerator::CalendarGenerator(void)
{
}

CalendarGenerator::~CalendarGenerator(void)
{
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

void CalendarGenerator::Generate(string path, Calendar& cal)
{
	vector<string> vecMonthDirs = GetFiles(path, "*.*", true);

	for(int i = 0; i < vecMonthDirs.size(); i++)
	{
		vector<string> vecDayDirs = GetFiles(path + "\\"
											+ vecMonthDirs[i], "*.*", true);

		for(int j = 0; j < vecDayDirs.size(); j++)
		{
			int date = atoi(vecDayDirs[j].c_str());
			cal[date] = date;
		}
	}
}

int Calendar::GetPrev(int today)
{
	map<int, int>::iterator it;

	it = this->find(today);

	if(it != this->begin())
	{
		it--;
		return it->first;
	}

	return -1;
}

int Calendar::GetNext(int today)
{
	map<int, int>::iterator it;

	it = find(today);

	if(it != this->end())
	{
		it++;
		if(it != this->end())
			return it->first;
	}

	return -1;
}