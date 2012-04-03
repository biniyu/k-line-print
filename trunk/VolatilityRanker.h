#pragma once

#include <vector>
#include <string>
#include <map>
#include "KLineCollection.h"

using namespace std;

class VolatilityRanker
{
public:

	enum RankMode
	{
		en_RankModeHighLow,
		en_RankModeOpenClose
	};

private:

	string							m_strFileName;
	map<string, KLineCollection>	m_mapDayLines;
	multimap <int, string>			m_mapRankList;
	RankMode						m_enRankMode;

public:

	VolatilityRanker(string file) 
	{ 
		m_strFileName = file; 
		m_enRankMode = en_RankModeHighLow;
	}

	~VolatilityRanker(void);

	vector<string> Rank();
	void AddToRankList(int volatility, string file);
	KLineCollection& GetDayLineData(string dayLineFile);
	void SetRankMode(RankMode mode) { m_enRankMode = mode; }

};
