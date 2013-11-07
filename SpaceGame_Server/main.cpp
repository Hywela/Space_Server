
#include "TcpServer.h"


int main(int argc, char **argv)
{
	TcpServer *server;
	server = new TcpServer();
	server->main_loop();
	
		


    return 0;
}