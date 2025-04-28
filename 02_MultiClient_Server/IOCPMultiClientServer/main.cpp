#include "IOCPServer.h"

int main()
{
	IOCPServer server;
	if (server.Initserver(12345))
	{
		server.Run();
	}
	return 0;
}