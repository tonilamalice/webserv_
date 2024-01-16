#include "SocketCreation.hpp"

//Constructor
SocketCreation::SocketCreation()
{
	// this->address = {};
	// this->hints = {};
	 // this->bind_address = NULL;
	// this->socket_listen = 0;
	// this->host = NULL;
	// this->port = NULL;
}

int SocketCreation::create_socket(const char* host, const char* port)
{
	std::cout << "Configuring local address..." << std::endl;
	this->host = (char*)host;
	this->port = (char*)port;
	
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	this->hints.ai_family = AF_UNSPEC;
	this->hints.ai_addr = 0;
	// address.sin_addr.s_addr = inet_addr("192.168.129.1");
	this->hints.ai_socktype = SOCK_STREAM;
	this->hints.ai_flags = AI_PASSIVE;
	this->hints.ai_protocol = 0;

	// struct addrinfo *bind_address;
	if(getaddrinfo(this->host, this->port, &this->hints, &this->bind_address) != 0)
	{
		fprintf(stderr, "getaddrinfo() failed. (%d)\n", errno);
		exit(1);
	}
	std::cout << "Creating socket..." << std::endl;
	this->socket_listen = socket(this->bind_address->ai_family, this->bind_address->ai_socktype, this->bind_address->ai_protocol);
	if(this->socket_listen < 0)
	{
		fprintf(stderr, "socket() failed. (%d)\n", errno);
		exit(1);
	}
	std::cout << "Binding socket to local address..." << std::endl;
	int opt = 1;
	if(setsockopt(this->socket_listen, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		fprintf(stderr, "setsockopt() failed. (%d)\n", errno);
		exit(1);
	}
	
	if(bind(this->socket_listen, this->bind_address->ai_addr, this->bind_address->ai_addrlen) < 0)
	{
		fprintf(stderr, "bind() failed. (%d)\n", errno);
		exit(1);
	}
	freeaddrinfo(this->bind_address);

	std::cout << "Listening..." << std::endl;
	if(listen(this->socket_listen, 10) < 0)
	{
		fprintf(stderr, "listen() failed. (%d)\n", errno);
		exit(1);
	}
	return (this->socket_listen);
}

SocketCreation::SocketCreation(const SocketCreation &copy)
{
	this->address = copy.address;
	this->hints = copy.hints;
	this->bind_address = copy.bind_address;
	this->socket_listen = copy.socket_listen;
	this->host = copy.host;
	this->port = copy.port;
}

SocketCreation	&SocketCreation::operator=(SocketCreation &copy)
{
	this->address = copy.address;
	this->hints = copy.hints;
	this->bind_address = copy.bind_address;
	this->socket_listen = copy.socket_listen;
	this->host = copy.host;
	this->port = copy.port;
	return (*this);
}

//Deconstructor

SocketCreation::~SocketCreation()
{
	std::cout << "Closing listening socket..." << std::endl;
	close(this->socket_listen);
}

//Getters

struct sockaddr_in	SocketCreation::getaddress()
{
	return (this->address);
}

struct addrinfo		SocketCreation::gethints()
{
	return (this->hints);
}

struct addrinfo		*SocketCreation::getbind_address()
{
	return (this->bind_address);
}

int					SocketCreation::getsocket_listen()
{
	return (this->socket_listen);
}

char*				SocketCreation::gethost()
{
	return (this->host);
}

char*				SocketCreation::getport()
{
	return (this->port);
}

