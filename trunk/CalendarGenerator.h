#pragma once

#include <map>
#include <string>

using namespace std;

class Calendar : public map<int,int>
{
public:
	int GetPrev(int today);
	int GetNext(int today);
};

class CalendarGenerator
{
public:
	CalendarGenerator(void);
	~CalendarGenerator(void);

	void Generate(string path, Calendar& cal);

};
