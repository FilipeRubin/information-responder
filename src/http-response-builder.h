#pragma once
#include "data/content-type.h"
#include <string>

class HTTPResponseBuilder
{
public:
	static std::string BuildResponseString(int statusCode, ContentType contentType, const std::string& content);
private:
	static std::string GetContentTypeMessage(ContentType contentType);
	static std::string GetStatusCodeMessage(int statusCode);
};