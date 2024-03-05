#pragma once
#include "i-message-logger.h"

class ConsoleMessageLogger : public IMessageLogger
{
public:
	void LogInfo(const std::string& message) override;
	void LogWarning(const std::string& message) override;
	void LogError(const std::string& message) override;
private:
	void PrintMessage(const std::string& prefix, const std::string& message) const;
};