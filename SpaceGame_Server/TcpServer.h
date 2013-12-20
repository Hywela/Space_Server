#pragma once
#include <SDL.h>
#include <SDL_net.h>
#include "MySqlHandler.h"
#include "User.h"
#pragma comment(lib,"SDL2.lib")
#pragma comment(lib,"SDL2main.lib")
#pragma comment(lib,"SDL2_net.lib")


const string SERVER_NOT_FULL = "OK";
const string SERVER_FULL     = "FULL";

           
const unsigned short BUFFER_SIZE = 1024;             // Size of our message buffer
const unsigned short MAX_SOCKETS = 6;               // Max number of sockets
const unsigned short MAX_CLIENTS = MAX_SOCKETS - 1; // Max number of clients in our socket set (-1 because server's listening socket takes the 1st socket in the set)

class TcpServer{
private:
	//Variables
	unsigned short PORT;				// The port our server will listen for incoming connecions on
	bool shutdownServer;				// true to shut the serverDown currently not used to shut it down but to keep it online
	User *user[MAX_CLIENTS];			// array with users , should be a vector or some que
	IPaddress serverIP;					// Server Ip
	TCPsocket serverSocket;              // The server socket that clients will use to connect to us
    TCPsocket clientSocket[MAX_CLIENTS]; // An array of sockets for the clients, we don't include the server socket (it's specified separately in the line above)
	bool socketIsFree[MAX_CLIENTS]; // An array of flags to keep track of which client sockets are free 
    int receivedByteCount;           
    int clientCount;                 // Count of how many clients are currently connected to the server
	char buffer[BUFFER_SIZE];            // Array of characters used to store the messages we receive
	SDLNet_SocketSet socketSet;		// This is an opaque data type used for socket sets
	int hostResolved;				// gets a value from SDL_hostresolved.. 
	//DataBase
	MySqlHandler *db;
	//Methods
	int GetRandomNumber(int nLow, int nHigh);
public:
	TcpServer(unsigned short PORT);
	~TcpServer();
	// Handles most of the incomming packages
	void handler(char switch_char, int clientNumber);
	//Networking Methods
	bool socket_activity();
	void socket_server_dataRecived();
	void socket_activity_all();
	void main_loop();
	void send(int clientNumber, string message); // send a messahe to a client
	void sendAttack(int clientNumber, char *message); // send a messahe to a client
	void dissconect(int clientNumber);			 // disconnects a client
	void matchMaking();							// Match two opponents
};

