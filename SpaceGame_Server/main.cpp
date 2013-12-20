// Space Game Server 
#include "TcpServer.h"
int main(int argc, char* argv[])
{
	cout << "Hello and Welcome to Space Game Server\n"<<
		"please input the PORT(standar(8881) if 0)\n"<<
		"PORT:";
	int PORT;
	cin >> PORT;
	if(PORT < 999) PORT = 8881; 

	cout << "\n Server is now using PORT" << PORT << endl;

	TcpServer *server; 
	server = new TcpServer(PORT);
	server->main_loop();
	
	
    return 0;
}