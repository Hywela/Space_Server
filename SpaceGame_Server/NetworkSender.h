#pragma once

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <SDL.h>
#include <SDL_net.h>


class NetworkSender
{

		UDPsocket udpsock; /* Socket descriptor */
        IPaddress server_address; /* the server address */
        UDPpacket *packet; /* Pointer to packet memory */
        int quit;
        int x;
        int y;
		char * server_name;
		Uint16 port_number;
		Uint16 default_client_port;


public:
	NetworkSender(Uint16 server_name, Uint16 port, int x, int y);
	~NetworkSender(void);
	NetworkSender();
	void handler_init(int argc, char **argv);
	void handler_send(char *input);
	void handlet_setHost(IPaddress address,UDPsocket udpsock);
};

