#pragma once

#include <map>
#include <string>

using namespace std;

class Calendar : public map<int,int>
{
public:
	int GetPrev(int today);
	int GetNext(int today);

	void Add(int nDate);
	int GetFirst();
	int GetLast();

	//	��˳�򷵻ص�nSeq����ֵ
	int GetBySeq(int nSeq);
};

class CalendarGenerator
{
public:
	CalendarGenerator(void);
	~CalendarGenerator(void);

	void Generate(string path, Calendar& cal);

};
