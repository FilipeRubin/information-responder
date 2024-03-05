#include "http-response-builder.h"
#include <sstream>

std::string HTTPResponseBuilder::BuildResponseString(int statusCode, ContentType contentType, const std::string& content)
{
    std::stringstream response;

    response << "HTTP/1.1 " << std::to_string(statusCode) << ' ' << GetStatusCodeMessage(statusCode) << "\r\n";
    response << "Content-Type: " << GetContentTypeMessage(contentType) << "\r\n";
    response << "Content-Length: " << content.size() << "\r\n\r\n";
    response << content;

    return response.str();
}

std::string HTTPResponseBuilder::GetContentTypeMessage(ContentType contentType)
{
    switch (contentType)
    {
    case ContentType::PLAIN_TEXT:
        return "text/plain";
    case ContentType::HTML:
        return "text/html";
    case ContentType::CSS:
        return "text/css";
    case ContentType::JS:
        return "application/javascript";
    default:
        return "text/plain";
    }
}

std::string HTTPResponseBuilder::GetStatusCodeMessage(int statusCode)
{
    switch (statusCode)
    {
    case 200:
        return "OK";
    case 404:
        return "Not Found";
    default:
        return "Unknown";
    }
}
