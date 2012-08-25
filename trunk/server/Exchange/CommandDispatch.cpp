#include "CommandDispatch.h"

CommandDispatch::CommandDispatch(void)
{
}

CommandDispatch::~CommandDispatch(void)
{
}

bool CommandDispatch::Register(int nCmdId, CommandHandler* pCmdHandler)
{
	m_mapHandlers[nCmdId] = pCmdHandler;
	return true;
}


bool CommandDispatch::Execute(int nCmdId, unsigned char pCmdBuf[], int nCmdSize,
							  unsigned char pAckBuf[], int& nAckSize)
{
	if(m_mapHandlers.find(nCmdId) != m_mapHandlers.end())
		return m_mapHandlers[nCmdId]->Execute(pCmdBuf, nCmdSize, pAckBuf, nAckSize);	
	else
		return false;
}