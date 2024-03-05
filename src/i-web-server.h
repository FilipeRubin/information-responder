#pragma once
#include <string>

class IWebServer
{
public:
	virtual ~IWebServer() = default;
	virtual void Initialize() = 0;
	virtual void Run() = 0;
	virtual void SetAddress(const std::string& address, unsigned short port) = 0;
};