#pragma once
#include "data/http-request.h"

class HTTPRequestParser
{
public:
	static HTTPRequest ParseRequest(const std::string& requestAsString);
private:
	static HTTPMethod GetMethodInLine(const std::string& line);
	static std::string GetFileRequestedInLine(const std::string& line);
	static bool IsNewlineLine(const std::string& line);
};