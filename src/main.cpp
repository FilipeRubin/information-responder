#include "console-message-logger.h"
#include "http-response-builder.h"
#include "rubin-web-server.h"

#pragma comment(lib, "WS2_32.lib")

#define RUBIN_DEFAULT_ADDRESS "192.168.0.127"
#define RUBIN_DEFAULT_PORT (unsigned short)30412

using std::move;
using std::make_unique;

int main()
{
	RubinWebServer webServer(move(make_unique<ConsoleMessageLogger>()));
	webServer.SetAddress(RUBIN_DEFAULT_ADDRESS, RUBIN_DEFAULT_PORT);
	webServer.Initialize();
	webServer.Run();

	return 0;
}