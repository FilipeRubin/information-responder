#pragma once
#include "i-web-server.h"
#include "directory-file-reader.h"
#include "data/http-request.h"
#include "i-message-logger.h"
#include <WinSock2.h>
#include <memory>

class RubinWebServer : public IWebServer
{
public:
	RubinWebServer(std::unique_ptr<IMessageLogger>&& messageLogger);
	void Initialize() override;
	void Run() override;
	void SetAddress(const std::string& address, unsigned short port) override;
private:
	int BindSocket();
	int CreateSocket();
	void FinalizeWinSock2();
	int Listen();
	void LoopAcceptConnections();
	int ProcessRequest(const HTTPRequest& request, SOCKET& out_clientSocket, std::string& out_responseAsString);
	int ResolveAddress();
	void SendResponse(const SOCKET& clientSocket, const std::string& response);
	void ShowStartupMessage();
	int StartupWinSock2();
	int WaitAndAcceptConnection(SOCKET& out_clientSocket, HTTPRequest& out_request);
	std::unique_ptr<IMessageLogger> m_messageLogger;
	DirectoryFileReader m_directoryFileReader;
	std::string m_address;
	unsigned short m_port;
	WSADATA m_wsaData;
	SOCKET m_listenSocket;
	SOCKADDR_IN m_socketAddress;
};