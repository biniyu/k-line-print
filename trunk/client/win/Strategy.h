#pragma once

#include "TickReader.h"
#include "KLineCollection.h"

class Strategy
{
public:
	Strategy(void);
	~Strategy(void);

	void SetData(KLineCollection* p1MinData)
	{
		m_p1MinData = p1MinData;
	}

	virtual void Quote(Tick tick);

private:

	KLineCollection* m_p1MinData;

	int m_lasttime;
	int m_lastprice;
};
