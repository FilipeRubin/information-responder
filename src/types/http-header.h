#pragma once
#include "http-method.h"
#include <string>

class HTTPHeader
{
public:
	HTTPHeader(HTTPMethod method, const std::string& requestedFile);
	HTTPMethod GetMethod() const;
	std::string GetRequestedFile() const;
private:
	HTTPMethod m_method;
	std::string m_requestedFile;
};