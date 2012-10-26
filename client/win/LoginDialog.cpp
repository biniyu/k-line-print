// LoginDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "KLinePrint.h"
#include "LoginDialog.h"

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

using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::WebSocket;
using Poco::Net::WebSocketException;

// CLoginDialog 对话框

IMPLEMENT_DYNAMIC(CLoginDialog, CDialog)

CLoginDialog::CLoginDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDialog::IDD, pParent)
{

}

CLoginDialog::~CLoginDialog()
{
}

void CLoginDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLoginDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CLoginDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CLoginDialog 消息处理程序

void CLoginDialog::OnBnClickedOk()
{
	HTTPClientSession cs("localhost", 8080);
	HTTPRequest request(HTTPRequest::HTTP_GET, "/ws");
	HTTPResponse response;
	WebSocket ws(cs, request, response);
	std::string payload("login");
	ws.sendFrame(payload.data(), payload.size());
	char buffer[1024];
	int flags;
	int n = ws.receiveFrame(buffer, sizeof(buffer), flags);
	assert (n == payload.size());
	assert (payload.compare(0, payload.size(), buffer, 0, n) == 0);
	assert (flags == WebSocket::FRAME_TEXT);
	
	payload = "Hello, universe!";
	ws.sendFrame(payload.data(), payload.size(), WebSocket::FRAME_BINARY);
	n = ws.receiveFrame(buffer, sizeof(buffer), flags);
	assert (n == payload.size());
	assert (payload.compare(0, payload.size(), buffer, 0, n) == 0);
	assert (flags == WebSocket::FRAME_BINARY);	
	
	ws.shutdown();
	n = ws.receiveFrame(buffer, sizeof(buffer), flags);

	OnOK();
}
