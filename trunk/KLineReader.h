#pragma once

#include <string>

using namespace std;

class KLineCollection;

class KLineReader
{
public:
	KLineReader(void);
	~KLineReader(void);

	void Read(string path, KLineCollection& klines);
};
