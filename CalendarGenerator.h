#pragma once

#include <map>
#include <vector>
#include <string>

using namespace std;

class Calendar : public map<int,int>
{
public:
	int GetPrev(int today);
	int GetNext(int today);
};

vector<string> GetFiles(string dir, string filter, bool bDir);

class CalendarGenerator
{
public:
	CalendarGenerator(void);
	~CalendarGenerator(void);

	void Generate(string path, Calendar& cal);

};
