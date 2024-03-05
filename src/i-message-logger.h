#pragma once
#include <string>

class IMessageLogger
{
public:
	virtual ~IMessageLogger() = default;
	virtual void LogInfo(const std::string& message) = 0;
	virtual void LogWarning(const std::string& message) = 0;
	virtual void LogError(const std::string& message) = 0;
};