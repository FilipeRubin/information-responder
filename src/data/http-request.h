#pragma once
#include "http-header.h"
#include <string>

class HTTPRequest
{
public:
	HTTPRequest();
	HTTPRequest(HTTPHeader header, const std::string& body);
	const std::string& GetBody() const;
	const HTTPHeader& GetHeader() const;
private:
	HTTPHeader m_header;
	std::string m_body;
};