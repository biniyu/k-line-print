#pragma once

#include <vector>
#include <string>
using namespace std;

class Tick
{
public:
	int time;
	int price;
	int vol;
};


class TickCollection : public vector<Tick> 
{
public:
	int open;
	int close;
	int high;
	int low;
	int avgvol;
};

class TickReader
{
public:
	TickReader(void);
	~TickReader(void);

	void Read(string path, TickCollection& ticks);
};
