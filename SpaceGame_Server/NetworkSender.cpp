#include "NetworkSender.h"


NetworkSender::NetworkSender(){
	

}
NetworkSender::NetworkSender(Uint16 server_name, Uint16 port, int id)
{

		this->id=id;
		
		this->server_name = "127.000.000.001";  // This is the default server name, and creates enough memory to store other names from argv[1]
		port_number = 8810;  // This is a default port that is overridden by argv[2]
		//server_address = server_name;  // This is the default server name, and creates enough memory to store other names from argv[1]
		//port_number = port;  // This is a default port that is overridden by argv[2]
		default_client_port = 8881; // set to 0 for a random port


}


NetworkSender::~NetworkSender(void)
{
	SDLNet_FreePacket(packet);

}


void NetworkSender::handler_init(int argc, char **argv)
{
	
				/* Open a socket */

 

	
	/* Make space for the packet */
	if (!(packet = SDLNet_AllocPacket(512)))
	{
		fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
	     /* Resolve server name into server address */
        if (SDLNet_ResolveHost(&server_address, server_name, port_number) == -1)
        {
                fprintf(stderr, "SDLNet_ResolveHost(%s %d): %s\n",server_name ,port_number, SDLNet_GetError());
                exit(EXIT_FAILURE);
        }
  


}


void NetworkSender::handler_send(char *input)
{
		

	strcpy_s((char *)packet->data, strlen(input)+1, input);
	
				packet->data[8]=(Uint8)id;
                packet->data[9]=(Uint8)0;
                packet->data[10]=(Uint8)0;
                packet->data[11]=(Uint8)0;
           
 
		packet->address.host = server_address.host;	/* Set the destination host */
		packet->address.port = server_address.port;	/* And destination port */
		
		packet->len = 24;
		SDLNet_UDP_Send(udpsock, -1, packet); /* This sets the p->channel */
 
		
	
	
 

}


void NetworkSender::handlet_setHost(IPaddress address ,UDPsocket udpsock)
{
	server_address.host = address.host;

	this->udpsock= udpsock;


}
