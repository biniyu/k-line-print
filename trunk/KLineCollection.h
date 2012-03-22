#pragma once

#include <vector>

using namespace std;

class TickCollection;

class KLine
{
public:
	int time;
	int high;
	int low;
	int open;
	int close;
	int vol;
	int avg;	// 均价线
};

class KLineCollection : public vector<KLine>
{
public:
	KLineCollection(void);
	~KLineCollection(void);

	int high;
	int low;

	int maxvol;
	int avgvol;

	//	需要显示的范围
	int startidx;
	int endidx;
	int curidx;

	//	放大K线图
	void Inflate();

	//	缩小K线图
	void Deflate();

	void MovePrev();

	void MoveNext();

	void GetHighLowPrice();

	//	从分笔数据生成指定周期(以秒为单位)的K线数据
	void Generate(TickCollection& ticks, int seconds, KLine prevDayLine);
};
