#pragma once
#include <SDL.h>
#include <SDL_net.h>
#include "MySqlHandler.h"
#include "NetworkSender.h"
#include "User.h"
#include <vector>
#pragma comment(lib,"SDL2.lib")
#pragma comment(lib,"SDL2main.lib")
#pragma comment(lib,"SDL2_net.lib")


const string SERVER_NOT_FULL = "OK";
const string SERVER_FULL     = "FULL";

const unsigned short PORT        = 1234;            // The port our server will listen for incoming connecions on
const unsigned short BUFFER_SIZE = 512;             // Size of our message buffer
const unsigned short MAX_SOCKETS = 5;               // Max number of sockets
const unsigned short MAX_CLIENTS = MAX_SOCKETS - 1; // Max number of clients in our socket set (-1 because server's listening socket takes the 1st socket in the set)

class TcpServer
{

	bool shutdownServer;
	User *user[MAX_CLIENTS];
	IPaddress serverIP; 
	TCPsocket serverSocket;              // The server socket that clients will use to connect to us
    TCPsocket clientSocket[MAX_CLIENTS]; // An array of sockets for the clients, we don't include the server socket (it's specified separately in the line above)
	
	bool      socketIsFree[MAX_CLIENTS]; // An array of flags to keep track of which client sockets are free 
    int receivedByteCount;           
    int clientCount;                 // Count of how many clients are currently connected to the server
	char buffer[BUFFER_SIZE];            // Array of characters used to store the messages we receive
	SDLNet_SocketSet socketSet;
	int hostResolved ;
	char dbHandler;
	MySqlHandler *db;
public:
	TcpServer();
	~TcpServer();

	void handler(char switch_char, int clientNumber);

	//tcp
	void socket_activity();
	void socket_server_dataRecived();
	void socket_activity_all();
	void main_loop();
	void send(int clientNumber, string message);
	void dissconect(int clientNumber);
	//match
	void matchMaking();
};

