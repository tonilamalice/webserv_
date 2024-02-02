#ifndef SocketCreation_hpp
#define SocketCreation_hpp

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <vector>
#include <algorithm>
#include <sstream>

#define PORT 8081;

class SocketCreation
{
	private:
		struct sockaddr_in address;
		struct addrinfo hints;
		struct addrinfo *bind_address;
			int socket_listen;
			char* host;
			char* port;
		
	public:
			//Constructor
			SocketCreation();
			int create_socket(const char* host,const char* port);
			SocketCreation(const SocketCreation &copy);
			SocketCreation	&operator=(SocketCreation &copy);
			
			//Deconstructor
			virtual ~SocketCreation();
			//Getters
		struct sockaddr_in	getaddress();
		struct addrinfo		gethints();
		struct addrinfo		*getbind_address();
		int					getsocket_listen();
		char*				gethost();
		char*				getport();
};


#endif