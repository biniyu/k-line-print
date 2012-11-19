#pragma once

#include <string>
#include <deque>
#include <map>
#include "KLineCollection.h"

using namespace std;

class KLineWriter
{
public:
	KLineWriter(void);
	~KLineWriter(void);

	void Write(string path, KLineCollection& klines);
};
