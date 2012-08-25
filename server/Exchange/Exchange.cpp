// Exchange.cpp : 定义控制台应用程序的入口点。
//

#include "ace/INET_Addr.h"
#include "ace/Log_Msg.h"
#include "ace/Asynch_Acceptor.h"
#include "ace/Asynch_IO.h"
#include "ace/Proactor.h"
#include "ExchangeService.h"
#include "CommandDispatch.h"
#include "CommandHandler.h"
#include "LoginHandler.h"

#define CMDID_LOGIN		1
#define CMDID_LOGOUT	2

class Exchange_Proactive_Acceptor :
    public ACE_Asynch_Acceptor<ExchangeService>
{
public:
  virtual int validate_connection
    (const ACE_Asynch_Accept::Result& result,
     const ACE_INET_Addr &remote,
     const ACE_INET_Addr &local);
};

int
Exchange_Proactive_Acceptor::validate_connection (
   const ACE_Asynch_Accept::Result&,
   const ACE_INET_Addr& remote,
   const ACE_INET_Addr& local)
{
	return 0;	
}

int ACE_TMAIN (int, ACE_TCHAR *[])
{
	LoginHandler login;

	g_dispatch.Register(CMDID_LOGIN, &login);

	ACE_INET_Addr listen_addr(12345);     // Set up with listen port

	Exchange_Proactive_Acceptor aio_acceptor;
	
	if (0 != aio_acceptor.open (listen_addr,
								0,     // bytes_to_read
								1))     // pass_addresses
	  ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
						 ACE_TEXT ("acceptor open")), 1);

	int success = 1;

	while (success > 0)
	{
		// Dispatch events via Proactor singleton.
		success = ACE_Proactor::instance ()->handle_events ();
	}

	return 0;
}
