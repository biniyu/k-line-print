#pragma once

#include <vector>
#include <string>
#include <map>
#include "KLineCollection.h"

using namespace std;

class VolatilityRanker
{
private:
	string							m_strFileName;
	map<string, KLineCollection>	m_mapDayLines;
	multimap <int, string>			m_mapRankList;

public:
	VolatilityRanker(string file) { m_strFileName = file; }
	~VolatilityRanker(void);

	vector<string> Rank();
	void AddToRankList(int volatility, string file);
	KLineCollection& GetDayLineData(string dayLineFile);

};
