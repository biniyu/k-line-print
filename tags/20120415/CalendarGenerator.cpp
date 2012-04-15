#include "StdAfx.h"
#include <vector>
#include <string>
#include "CalendarGenerator.h"
#include "DataRepoUtil.h"

using namespace std;

CalendarGenerator::CalendarGenerator(void)
{
}

CalendarGenerator::~CalendarGenerator(void)
{
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