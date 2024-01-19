#ifndef Client_hpp
#define Client_hpp

#include "Utils.hpp"
#include "SocketCreation.hpp"
#include <map>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#define MAX_REQUEST_SIZE 2047

class Utils;

class Client : public SocketCreation
{
private:
public:
	std::string rootDirectory;
	std::string rootupdated;
	Client();
	~Client();
	
	struct client_info{
		socklen_t address_length;
		struct sockaddr_storage address;
		char address_buffer[128];
		int socket;
		char request[MAX_REQUEST_SIZE + 1];
		int received;
		struct client_info *next;
	};

	struct client_info *get_client(struct client_info **client_list, int s);
	void drop_client(struct client_info **client_list, struct client_info *client);
	const char *get_client_address(struct client_info *ci);
	fd_set wait_on_clients(struct client_info **client_list, int server);
	void send_400(struct client_info **client_list, struct client_info *client);
	void send_404(struct client_info **client_list, struct client_info *client);
	void send_500(struct client_info **client_list, struct client_info *client);
	void send_501(struct client_info **client_list, struct client_info *client);
	void serve_resource(struct client_info **client_list, struct client_info *client, std::string path2);
	void getFilesInDirectory(std::vector<std::string> &files);
	std::ostringstream sendFileList();
	void downloadFile(const std::string& filename,struct client_info **client_list, struct client_info *client); 
	void postmethod(const char* request, struct client_info **client_list, struct client_info *client);
	void deletemethod(std::string filePath, struct client_info **client_list, struct client_info *client);
};




#endif