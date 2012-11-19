#include "StdAfx.h"
#include <vector>
#include <string>
#include "CalendarGenerator.h"
#include "Utility.h"

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
			cal.Add(date);
		}
	}
}

void Calendar::Add(int nDate)
{
	(*this)[nDate] = nDate;
}

int Calendar::GetFirst()
{
	if(0 == size()) return -1;

	map<int,int>::iterator it;

	it = this->begin();

	return it->first;
}

int Calendar::GetLast()
{
	map<int,int>::reverse_iterator rit;

	rit = this->rbegin();

	return rit->first;
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
	else	//	TODO : 必须能处理找不到当前日期的情况
	{

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
	else	//	TODO : 必须能处理找不到当前日期的情况
	{

	}

	return -1;
}

int Calendar::GetBySeq(int nSeq)
{
	if(nSeq >= this->size()) return -1;

	int nCurIdx = 0;

	map<int,int>::iterator it;

	for(it = this->begin(); it != this->end(); it++)
	{
		if(nCurIdx == nSeq) return it->first;
		nCurIdx++;
	}

	return -1;
}
