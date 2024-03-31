#include "http-header.h"

HTTPHeader::HTTPHeader(HTTPMethod method, const std::string& requestedFile) :
	m_method(method),
	m_requestedFile(requestedFile)
{
}

HTTPMethod HTTPHeader::GetMethod() const
{
	return m_method;
}

std::string HTTPHeader::GetRequestedFile() const
{
	return m_requestedFile;
}
