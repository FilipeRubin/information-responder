#include "http-request-parser.h"
#include <sstream>

HTTPRequest HTTPRequestParser::ParseRequest(const std::string& requestAsString)
{
	if (requestAsString.size() == 0)
	{
		return HTTPRequest(HTTPHeader(HTTPMethod::INVALID, ""), "");
	}

	HTTPMethod resultMethod = HTTPMethod::INVALID;
	std::string resultRequestedFile = "";

	bool firstLineRead = false;
	bool headerRead = false;

	std::stringstream requestStream = std::stringstream(requestAsString);
	std::string line;
	std::stringstream bodyStream;

	while (std::getline(requestStream, line))
	{
		if (not firstLineRead)
		{
			resultMethod = GetMethodInLine(line);
			firstLineRead = true;
			if (resultMethod == HTTPMethod::INVALID)
			{
				break;
			}
			resultRequestedFile = GetFileRequestedInLine(line);
		}

		if (not headerRead and IsNewlineLine(line))
		{
			headerRead = true;
		}
		else if (headerRead)
		{
			bodyStream << line;
		}
	}

	return HTTPRequest(HTTPHeader(resultMethod, resultRequestedFile), bodyStream.str());
}

HTTPMethod HTTPRequestParser::GetMethodInLine(const std::string& line)
{
	if (line.starts_with("GET"))
	{
		return HTTPMethod::GET;
	}
	else if (line.starts_with("POST"))
	{
		return HTTPMethod::POST;
	}
	return HTTPMethod::INVALID;
}

std::string HTTPRequestParser::GetFileRequestedInLine(const std::string& line)
{
	size_t startCharPos = line.find('/');

	if (startCharPos == std::string::npos)
		return "";
	
	size_t endCharPos = line.find(' ', startCharPos);

	if (endCharPos == std::string::npos)
		return "";
	
	size_t length = endCharPos - startCharPos;
	return line.substr(startCharPos, length);
}

bool HTTPRequestParser::IsNewlineLine(const std::string& line)
{
	return line == "\r\n" or line == "\n" or line == "\r";
}
