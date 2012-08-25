#pragma once

class CommandHandler
{
public:
	CommandHandler(void);
	~CommandHandler(void);

	virtual bool Execute(unsigned char pCmdBuf[], int nCmdSize,
						unsigned char pAckBuf[], int& nAckSize) = 0;
};
