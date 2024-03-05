#include "rubin-web-server.h"
#include "http-request-parser.h"
#include "http-response-builder.h"
#include <WS2tcpip.h>

#define DEFAULT_SERVER_DIRECTORY "server-files"

RubinWebServer::RubinWebServer(std::unique_ptr<IMessageLogger>&& messageLogger) :
	m_messageLogger(std::move(messageLogger)),
	m_directoryFileReader(DirectoryFileReader(DEFAULT_SERVER_DIRECTORY)),
	m_address(std::string("")),
	m_port(0U),
	m_wsaData(WSADATA()),
	m_listenSocket(0ULL),
	m_socketAddress(SOCKADDR_IN())
{
}

void RubinWebServer::Initialize()
{
	if (StartupWinSock2() != 0)
	{
		m_messageLogger->LogError("WinSock2 could not be initialized.");
		return;
	}

	if (CreateSocket() != 0)
	{
		m_messageLogger->LogError("Socket could not be created.");
		return;
	}

	if (ResolveAddress() != 0)
	{
		m_messageLogger->LogError("Could not resolve address.");
		return;
	}

	if (BindSocket() != 0)
	{
		m_messageLogger->LogError("Could not bind socket.");
		return;
	}
}

void RubinWebServer::Run()
{
	if (Listen() != 0)
	{
		m_messageLogger->LogError("[Error] Could not listen.");
		return;
	}
	ShowStartupMessage();
	LoopAcceptConnections();
	FinalizeWinSock2();
}

void RubinWebServer::SetAddress(const std::string& address, unsigned short port)
{
	m_address = address;
	m_port = port;
}

int RubinWebServer::BindSocket()
{
	int socketAddressLength = sizeof(m_socketAddress);
	if (bind(m_listenSocket, (SOCKADDR*)&m_socketAddress, socketAddressLength) != 0)
	{
		closesocket(m_listenSocket);
		WSACleanup();
		return 1;
	}
	return 0;
}

int RubinWebServer::CreateSocket()
{
	m_listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_listenSocket == INVALID_SOCKET)
	{
		WSACleanup();
		return 1;
	}
	return 0;
}

void RubinWebServer::FinalizeWinSock2()
{
	closesocket(m_listenSocket);
	WSACleanup();
	m_messageLogger->LogInfo("Rubin Web Server terminated successfully.");
}

int RubinWebServer::Listen()
{
	if (listen(m_listenSocket, 10) != 0)
	{
		closesocket(m_listenSocket);
		WSACleanup();
		return 1;
	}
	return 0;
}

void RubinWebServer::LoopAcceptConnections()
{
	while (true) // true?
	{
		SOCKET clientSocket;
		HTTPRequest request = HTTPRequest();

		if (WaitAndAcceptConnection(clientSocket, request) != 0)
			continue;

		std::string response;

		if (ProcessRequest(request, clientSocket, response) != 0)
			continue;
		
		SendResponse(clientSocket, response);
		closesocket(clientSocket);
	}
}

int RubinWebServer::ProcessRequest(const HTTPRequest& request, SOCKET& out_clientSocket, std::string& out_responseAsString)
{
	if (request.GetHeader().GetMethod() == HTTPMethod::INVALID)
	{
		closesocket(out_clientSocket);
		return 1;
	}
	else if (request.GetHeader().GetMethod() == HTTPMethod::GET)
	{
		std::string requestedFile = request.GetHeader().GetRequestedFile();
		if (requestedFile == "/")
		{
			requestedFile = "index.html";
		}
		bool fileExists = m_directoryFileReader.FileExists(requestedFile);
		int statusCode = fileExists ? 200 : 404;
		ContentType contentType = ContentType::PLAIN_TEXT;

		if (requestedFile.ends_with(".html"))
		{
			contentType = ContentType::HTML;
		}
		else if (requestedFile.ends_with(".css"))
		{
			contentType = ContentType::CSS;
		}
		else if (requestedFile.ends_with(".js"))
		{
			contentType = ContentType::JS;
		}

		std::string content;
		if (fileExists)
		{
			content = m_directoryFileReader.ReadAllFile(requestedFile);
		}
		else
		{
			content = "Error 404: Not found";
		}
		out_responseAsString = HTTPResponseBuilder::BuildResponseString(statusCode, contentType, content);
	}

	return 0;
}

int RubinWebServer::ResolveAddress()
{
	m_socketAddress.sin_family = AF_INET;
	m_socketAddress.sin_port = htons(m_port);

	if (inet_pton(AF_INET, m_address.c_str(), &m_socketAddress.sin_addr) != 1)
	{
		closesocket(m_listenSocket);
		WSACleanup();
		return 1;
	}
	return 0;
}

void RubinWebServer::SendResponse(const SOCKET& clientSocket, const std::string& response)
{
	int totalBytesSent = 0;

	while (totalBytesSent < response.size())
	{
		int bytesSent = send(clientSocket, response.c_str(), response.size(), 0);
		if (bytesSent < 0)
		{
			m_messageLogger->LogWarning("Could not send response.");
			continue;
		}
		totalBytesSent += bytesSent;
	}
	m_messageLogger->LogInfo("Response sent to client.");
}

void RubinWebServer::ShowStartupMessage()
{
	std::string startupMessage = "Rubin Web Server listening to " + m_address + ':' + std::to_string(m_port);
	m_messageLogger->LogInfo(startupMessage);
}

int RubinWebServer::StartupWinSock2()
{
	int startupResult = WSAStartup(MAKEWORD(2, 2), &m_wsaData);
	if (startupResult != 0)
	{
		return 1;
	}
	return 0;
}

int RubinWebServer::WaitAndAcceptConnection(SOCKET& out_clientSocket, HTTPRequest& out_request)
{
	int socketAddressLength = sizeof(m_socketAddress);
	out_clientSocket = accept(m_listenSocket, (SOCKADDR*)&m_socketAddress, &socketAddressLength);

	if (out_clientSocket == INVALID_SOCKET)
	{
		m_messageLogger->LogWarning("Could not accept client request.");
		return 1;
	}

	const size_t requestBufferLength = 512;
	char requestBuffer[requestBufferLength];

	int bytesReceived = recv(out_clientSocket, requestBuffer, requestBufferLength, 0);
	if (bytesReceived < 0 or bytesReceived == requestBufferLength)
	{
		m_messageLogger->LogWarning("Could not read client request.");
		closesocket(out_clientSocket);
		return 1;
	}

	std::string message = requestBuffer;
	message.resize(bytesReceived);
	out_request = HTTPRequestParser::ParseRequest(message);
	return 0;
}