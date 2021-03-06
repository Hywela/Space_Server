#include "TcpServer.h"
#include <cstdlib>	//rand and srand
#define nodebug		//debug all couts // realease no couts

TcpServer::TcpServer(unsigned short PORT){
	this->PORT = PORT;
	shutdownServer = false;
	// Database handling
	db = new MySqlHandler();	// Host , username, port , and password
	db->set_database_info("");
	db->init_connection();		// Initialize the database
	// zeroing out 
	receivedByteCount=0;		
	clientCount = 0;

	// Initialize the network API.
	if (SDLNet_Init() == -1){
		cout << "Failed to intialise SDL_net: " << SDLNet_GetError() << endl;
		exit(-1); // Quit!
	}

	// Create the socket set with enough space to store our desired number of connections (i.e. sockets)
	socketSet = SDLNet_AllocSocketSet(MAX_SOCKETS);

	if (socketSet == NULL){
		cout << "Failed to allocate the socket set: " << SDLNet_GetError() << "\n";
		exit(-1); // Quit!
	}
	else {
		cout << "Allocated socket set with size:  " << MAX_SOCKETS << ", of which " << MAX_CLIENTS << " are availble for use by clients." <<  endl;
	}

	// Initialize all the client sockets and users (i.e. blank them ready for use!)
	for (int loop = 0; loop < MAX_CLIENTS; loop++){
		user[loop] = NULL;
		clientSocket[loop] = NULL;
		socketIsFree[loop] = true; // Set all our sockets to be free (i.e. available for use for new client connections)
	}

	// Try to resolve the provided server hostname. If successful, this places the connection details in the serverIP object and creates a listening port on the provided port number
	// Note: Passing the second parameter as "NULL" means "make a listening port". SDLNet_ResolveHost returns one of two values: -1 if resolving failed, and 0 if resolving was successful
	hostResolved = SDLNet_ResolveHost(&serverIP, NULL, PORT);

	if (hostResolved == -1){
		cout << "Failed to resolve the server host: " << SDLNet_GetError() << endl;
	}
	else{ // If we resolved the host successfully, output the details
		// Get our IP address in proper dot-quad format by breaking up the 32-bit unsigned host address and splitting it into an array of four 8-bit unsigned numbers...
		Uint8 * dotQuad = (Uint8*)&serverIP.host;
		//... and then outputting them cast to integers. Then read the last 16 bits of the serverIP object to get the port number
		cout << "Successfully resolved server host to IP: " << (unsigned short)dotQuad[0] << "." << (unsigned short)dotQuad[1] << "." << (unsigned short)dotQuad[2] << "." << (unsigned short)dotQuad[3];
		cout << " port " << SDLNet_Read16(&serverIP.port) << endl << endl;
	}

	// Try to open the server socket	
	serverSocket = SDLNet_TCP_Open(&serverIP);
	if (!serverSocket){		
		cout << "Failed to open the server socket: " << SDLNet_GetError() << "\n";	
		exit(-1);//Quit
	}
	else{
		cout << "Sucessfully created server socket." << endl;
	}

	// Add our server socket to the socket set
	SDLNet_TCP_AddSocket(socketSet, serverSocket);
	cout << "Awaiting clients..." << endl;


}
TcpServer::~TcpServer(){
	// Free our socket set (i.e. all the clients in our socket set)
	SDLNet_FreeSocketSet(socketSet);

	// Close our server socket, cleanup SDL_net and finish!
	SDLNet_TCP_Close(serverSocket);
	SDLNet_Quit();

}
void TcpServer::socket_server_dataRecived(){
	int serverSocketActivity = SDLNet_SocketReady(serverSocket);
	if (serverSocketActivity != 0){
		// If we have room for more clients...
		if (clientCount < MAX_CLIENTS){
			// Find the first free socket in our array of client sockets
			int freeSpot = -99;
			for (int loop = 0; loop < MAX_CLIENTS; loop++){
				if (socketIsFree[loop] == true){
					//cout << "Found a free spot at element: " << loop << endl;
					socketIsFree[loop] = false; // Set the socket to be taken
					freeSpot = loop;            // Keep the location to add our connection at that index in the array of client sockets
					break;                      // Break out of the loop straight away
				}
			}
			// ...accept the client connection and then...
			clientSocket[freeSpot] = SDLNet_TCP_Accept(serverSocket);
			user[freeSpot] = new User(freeSpot);
			// ...add the new client socket to the socket set (i.e. the list of sockets we check for activity)
			SDLNet_TCP_AddSocket(socketSet, clientSocket[freeSpot]);
			// Increase our client count
			clientCount++;
			// Send a message to the client saying "OK" to indicate the incoming connection has been accepted
			strcpy_s( buffer, SERVER_NOT_FULL.c_str() );
			int msgLength = strlen(buffer) + 1;
			SDLNet_TCP_Send(clientSocket[freeSpot], (void *)buffer, msgLength);
#ifdef debug
			cout << "Client connected. There are now " << clientCount << " client(s) connected." << endl << endl;
#endif
		}
		else{ // If we don't have room for new clients...
#ifdef debug
			cout << "*** Maximum client count reached - rejecting client connection ***" << endl;
#endif
			// Accept the client connection to clear it from the incoming connections list
			TCPsocket tempSock = SDLNet_TCP_Accept(serverSocket);
			// Send a message to the client saying "FULL" to tell the client to go away
			strcpy_s( buffer, SERVER_FULL.c_str() );
			int msgLength = strlen(buffer) + 1;
			SDLNet_TCP_Send(tempSock, (void *)buffer, msgLength);
			// Shutdown, disconnect, and close the socket to the client
			SDLNet_TCP_Close(tempSock);
		}
	} // End of if server socket is has activity check
}
bool TcpServer::socket_activity(){
	//						(The socket set to check, 0 means no waiting)
	int numActiveSockets = SDLNet_CheckSockets(socketSet, 0);
	if (numActiveSockets != 0)
	{
		///cout << "There are currently " << numActiveSockets << " socket(s) with data to be processed." << endl;
	return true;
	}
	return false;
}
void TcpServer::socket_activity_all(){
	// Loop to check all possible client sockets for activity
	for (int clientNumber = 0; clientNumber < MAX_CLIENTS; clientNumber++)
	{
		// If the socket is ready (i.e. it has data we can read)... (SDLNet_SocketReady returns non-zero if there is activity on the socket, and zero if there is no activity)
		int clientSocketActivity = SDLNet_SocketReady(clientSocket[clientNumber]);
#ifdef debug
	//	cout << "Just checked client number " << clientNumber << " and received activity status is: " << clientSocketActivity << endl;
#endif
		// If there is any activity on the client socket...
		if (clientSocketActivity != 0){
			// Check if the client socket has transmitted any data by reading from the socket and placing it in the buffer character array
			receivedByteCount = SDLNet_TCP_Recv(clientSocket[clientNumber], buffer, BUFFER_SIZE);
			// If there's activity, but we didn't read anything from the client socket, then the client has disconnected...
			if (receivedByteCount <= 0)
			{
#ifdef debug	//...so output a suitable message and then...
				cout << "Client " << clientNumber << " disconnected." << endl << endl;
#endif
				//... remove the socket from the socket set, then close and reset the socket ready for re-use and finally...
				SDLNet_TCP_DelSocket(socketSet, clientSocket[clientNumber]);
				SDLNet_TCP_Close(clientSocket[clientNumber]);
				clientSocket[clientNumber] = NULL;

				// ...free up their slot so it can be reused...
				socketIsFree[clientNumber] = true;

				// ...and decrement the count of connected clients.
				clientCount--;
#ifdef debug
				cout << "Server is now connected to: " << clientCount << " client(s)." << endl << endl;
#endif
			}else{ // If we read some data from the client socket...
				// Output the message the server received to the screen
#ifdef debug	cout << "Received: >>>> " << buffer << " from client number: " << clientNumber << endl;
				cout << "Received: >>>> " << buffer << " from client number: " << clientNumber << endl;
#endif			//..Handles most of the client requests , char Switch based
				handler(buffer[0], clientNumber);
#ifdef debug
				// If the client told us to shut down the server, then set the flag to get us out of the main loop and shut down
				if ( strcmp(buffer, "shutdown") == 0 )
				{
					shutdownServer = true;
					cout << "Disconnecting all clients and shutting down the server..." << endl << endl;
				}
#endif
			} // End else - clientSocketActivity != 0

		} // End of if client socket is active check

	} // End of server socket check sockets loop
}
void TcpServer::main_loop(){

	do{	
		if(socket_activity()){	// checks if there are any sockets to process
		socket_activity_all();	// handles the sockets when there is activity
		socket_server_dataRecived();} // Accepts or denies connections
		if(clientCount >= 2)		// if the server need to check the que and match it up
			matchMaking();			// matches two users in a figth
	}while(shutdownServer ==false);
}
void TcpServer::send(int clientNumber , string message){
	strcpy_s( buffer,message.c_str() );
	int inputLength = strlen(buffer) + 1;

	// If the message length is more than 1 (i.e. client pressed enter without entering any other text), then
	// send the message to all connected clients except the client who originated the message in the first place
	if ( socketIsFree[clientNumber] == false)
	{
		//cout << "re message: " << buffer << " (" << inputLength << " bytes) to client number: " << clientNumber << endl;
		SDLNet_TCP_Send(clientSocket[clientNumber], (void *)buffer, inputLength);
	}


}
void TcpServer::sendAttack(int clientNumber , char *message){
	//strcpy_s( buffer, message );
	int inputLength = strlen(message) + 1;

	// If the message length is more than 1 (i.e. client pressed enter without entering any other text), then
	// send the message to all connected clients except the client who originated the message in the first place
	if ( socketIsFree[clientNumber] == false)
	{
		//cout << "re message: " << buffer << " (" << inputLength << " bytes) to client number: " << clientNumber << endl;
		SDLNet_TCP_Send(clientSocket[clientNumber], (void *)message, inputLength);
	}


}
void TcpServer::handler(char switch_char, int clientNumber){

	switch (switch_char)
	{
	case 'q':{

		dissconect(clientNumber);
			 }
			 break;
	case 'b':{
		string in(buffer);
		int idx = in.find("b");
		string ship = in.substr(idx+2); 
		db->saveShip( user[clientNumber]->getDBID() , ship);
		break;
			 }
	case 'c':{
		send(clientNumber, db->getShip(user[clientNumber]->getDBID()) );
		break;
			 }             
	case 'e':{
		send(clientNumber, db->getShip(user[user[clientNumber]->inFigthWith()]->getDBID()) );
			 }
			 break;
	case 'l':{

		string in(buffer);
		cout << in;
		int idx = in.find("/");
		int un = in.find("-");
		int ofset = ((idx-1)+un)-un-1;

		string uname = in.substr(un+1,ofset);
		string pwd = in.substr(idx+1); 
		cout << "\n\n--"<< uname << " " << pwd;
		int id = db->handler_getID(uname, pwd);

		if (id >0){
			user[clientNumber]->setUserName(uname, id);
			send(clientNumber,"OK");

		}else dissconect(clientNumber);
		break;}
			 //Sign up for the que leave the que call it again
	case 'm':{
		user[clientNumber]->setInQueForMatch();
		//	send(clientNumber,"q-inQue");
		break;}
			 //figth data
	case 'f':{
		if(user[clientNumber]->userInMatch()){
			int returnTO = user[clientNumber]->inFigthWith();
			sendAttack(returnTO,buffer);
		}
		break;}
	case 's':{

		break;}

	default: SDL_Delay(20);
		break;
	}

}
void TcpServer::dissconect(int clientNumber){

	//...so output a suitable message and then...
	cout << "Client " << clientNumber << " disconnected." << endl << endl;

	//... remove the socket from the socket set, then close and reset the socket ready for re-use and finally...
	SDLNet_TCP_DelSocket(socketSet, clientSocket[clientNumber]);
	SDLNet_TCP_Close(clientSocket[clientNumber]);
	clientSocket[clientNumber] = NULL;
	delete user[clientNumber];user[clientNumber] = NULL;
	// ...free up their slot so it can be reused...
	socketIsFree[clientNumber] = true;

	// ...and decrement the count of connected clients.
	clientCount--;
#ifdef debug
	cout << "Server is now connected to: " << clientCount << " client(s)." << endl << endl;
#endif

}
void TcpServer::matchMaking(){

	int playerOne = NULL;	// Player 1
	int playerTwo = NULL;	// player Two
	int numberOfPlayers = 0; // keep count on how many in que it found
							// Loops true all clients to see if they are in the que
	for (int i=0; i<MAX_CLIENTS; i++){
		if (user[i] != NULL){	// user exsist or not
			if(user[i]->inQueForMatch()){	// is it in que
				if(playerOne == NULL){		// if spot 1 is free
					numberOfPlayers++;		
					playerOne = i;
				}else if (playerTwo == NULL){// spot 2 is free
					numberOfPlayers++;
					playerTwo = i;
				}
				if (numberOfPlayers == 2){	// match them together
					//cout << " \n  playerone << " << playerOne << " playeTwo << " << playerTwo;
					user[playerOne]->setUserInMatch(playerTwo); // take it out of the que
					user[playerTwo]->setUserInMatch(playerOne); // and sets who they are in match with
					int random = GetRandomNumber(0,1);
					if(random == 0){//Player one	// who starts first and returns matchfound
						send(user[playerOne]->getClientNumber() ,"matchFound/YouAreStarting");
						send(user[playerTwo]->getClientNumber() ,"matchFound/EnemyStarting");
					}else if( random == 1) {						//player two
						send(user[playerOne]->getClientNumber() ,"matchFound/EnemyStarting");
						send(user[playerTwo]->getClientNumber() ,"matchFound/YouAreStarting");
					}	// sets variables to zero so we can match the next two
					numberOfPlayers=0; playerTwo = NULL ;playerOne = NULL;
				}// two players matched
			}//in que
		}//end if (user != NULL)
	}// End for()
}//End Method
int  TcpServer::GetRandomNumber(int nLow, int nHigh){
	//returns a number between nLow and nHigh
	return (rand() % (nHigh - nLow + 1)) + nLow;
}