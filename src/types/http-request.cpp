#include "http-request.h"

HTTPRequest::HTTPRequest() :
	m_header(HTTPMethod::INVALID, ""),
	m_body("")
{
}

HTTPRequest::HTTPRequest(HTTPHeader header, const std::string& body) :
	m_header(header),
	m_body(body)
{
}

const std::string& HTTPRequest::GetBody() const
{
	return m_body;
}

const HTTPHeader& HTTPRequest::GetHeader() const
{
	return m_header;
}
