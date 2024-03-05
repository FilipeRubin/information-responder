#include "console-message-logger.h"
#include <iostream>

void ConsoleMessageLogger::LogInfo(const std::string& message)
{
	PrintMessage("Info", message);
}

void ConsoleMessageLogger::LogWarning(const std::string& message)
{
	PrintMessage("Warning", message);
}

void ConsoleMessageLogger::LogError(const std::string& message)
{
	PrintMessage("Error", message);
}

void ConsoleMessageLogger::PrintMessage(const std::string& prefix, const std::string& message) const
{
	std::cout << '[' << prefix << "]: " << message << '\n';
}
