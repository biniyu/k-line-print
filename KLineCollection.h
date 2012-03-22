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
	int avg;	// ������
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

	//	��Ҫ��ʾ�ķ�Χ
	int startidx;
	int endidx;
	int curidx;

	//	�Ŵ�K��ͼ
	void Inflate();

	//	��СK��ͼ
	void Deflate();

	void MovePrev();

	void MoveNext();

	void GetHighLowPrice();

	//	�ӷֱ���������ָ������(����Ϊ��λ)��K������
	void Generate(TickCollection& ticks, int seconds, KLine prevDayLine);
};
