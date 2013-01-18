// console.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>

#include "Poco/Net/WebSocket.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/NetException.h"
#include "Poco/Thread.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Format.h"

using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::WebSocket;
using Poco::Net::WebSocketException;
using Poco::Net::NetException;
using Poco::Exception;
using Poco::Timestamp;
using Poco::Thread;
using Poco::ThreadPool;
using Poco::Runnable;
using Poco::Event;
using Poco::Util::ServerApplication;
using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;

using namespace std;

class MyRunnable: public Runnable
{
public:
	MyRunnable(): m_psock(NULL)
	{
	}

	void send(std::string msg)
	{
		if(m_psock)
		{
			printf("send msg %s\n", msg.c_str());
			m_psock->sendFrame(msg.data(), msg.size());
		}
		else
		{
			printf("sending when not connected!\n");
		}
	}
	
	void run()
	{
//		HTTPClientSession cs("oscar.iego.net", 9980);
		HTTPClientSession cs("localhost", 9980);
		HTTPRequest request(HTTPRequest::HTTP_GET, "/ws");
		HTTPResponse response;

	_retry:

		try
		{
			if(!m_psock)
				m_psock = new WebSocket(cs, request, response);

			printf("WebSocket connection established.\n");

			while(1)
			{
				char buffer[1024];
				int flags;
				int n = m_psock->receiveFrame(buffer, sizeof(buffer), flags);
				buffer[n]='\0';
				printf("received msg : %s\n", buffer);
			}
		}
		catch (NetException& exc)
		{
			printf("%d : %s\n", exc.code(), exc.displayText().c_str());
		
			if(m_psock)
			{
				delete m_psock;
				m_psock = NULL;
			}

			Poco::Thread::sleep(2000);
			goto _retry;
		}
		catch(Exception exc)
		{
			printf("Exception %s\n", exc.displayText().c_str());
		}
		catch(exception exc)
		{
			printf("exception %s\n", exc.what());
		}

		printf("done!\n");
	}

private:

	WebSocket* m_psock;

};

int _tmain(int argc, _TCHAR* argv[])
{
	Thread thread;
	MyRunnable r;

	thread.start(r);
	Thread::sleep(200);

	r.send("aaa");

	Thread::sleep(2000);

	std::string cmd;

	while(1)
	{
		cin >> cmd;
		r.send(cmd);
	}

	printf("wait join\n");
	thread.join();
	printf("joined.\n");

	return 0;
}

