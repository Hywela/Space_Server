
#include "TcpServer.h"
#include "function_pointers.h"


int main(int argc, char **argv)
{

	TcpServer *server;
	server = new TcpServer();
	server->main_loop();
	
    return 0;
}