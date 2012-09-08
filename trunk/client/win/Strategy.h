#pragma once

#include "TickReader.h"
#include "KLineCollection.h"

class Strategy
{
public:
	Strategy(void);
	~Strategy(void);

	void SetData(KLineCollection* p1MinData, KLineCollection* p5SecData)
	{
		m_p1MinData = p1MinData;
		m_p5SecData = p5SecData;
	}

	virtual void Quote(Tick tick);

private:

	KLineCollection* m_p1MinData;
	KLineCollection* m_p5SecData;

	int m_lasttime;
	int m_lastprice;
};
