#pragma once
#include "commandhandler.h"

class LoginHandler :
	public CommandHandler
{
public:
	LoginHandler(void);
	~LoginHandler(void);

	virtual bool Execute(unsigned char pCmdBuf[], int nCmdSize,
						unsigned char pAckBuf[], int& nAckSize);

};
