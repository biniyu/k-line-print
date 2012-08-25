#pragma once

#include <map>
#include "CommandHandler.h"

using namespace std;

class CommandDispatch
{
public:
	CommandDispatch(void);
	~CommandDispatch(void);

	//	×¢²á´¦ÀíÆ÷
	bool Register(int nCmdId, CommandHandler* pCmdHandler);

	//	Ö´ĞĞÃüÁî	
	bool Execute(int nCmdId, unsigned char pCmdBuf[], int nCmdSize,
				 unsigned char pAckBuf[], int& nAckSize);

private:

	map<int, CommandHandler*> m_mapHandlers;

};
