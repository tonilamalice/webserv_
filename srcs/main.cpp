#include"Client.hpp"
#include"SocketCreation.hpp"

int main()
{
	Client connection;
	SocketCreation socket;
	int server = socket.create_socket(0, "8082");
	
	struct Client::client_info *client_list = NULL;

	while(1) 
	{
		fd_set reads;
		reads = connection.wait_on_clients(&client_list, server);

		if (FD_ISSET(server, &reads)) 
		{
			struct Client::client_info *client = connection.get_client(&client_list, -1);
			client->socket = accept(server, (struct sockaddr*) &(client->address), &(client->address_length));
			if (client->socket < 0) 
			{
				fprintf(stderr, "accept() failed. (%d)\n", errno);
				return 1;
			}
		std::cout << "New connection from " << connection.get_client_address(client) << std::endl;
		}
		
		struct Client::client_info *client = client_list;
		while(client) 
		{
			struct Client::client_info *next = client->next;
			if (FD_ISSET(client->socket, &reads)) 
			{
				if (MAX_REQUEST_SIZE == client->received) 
				{
					connection.send_400(&client_list, client);
					client = next;
					continue;
				}

				int r = recv(client->socket, client->request + client->received, MAX_REQUEST_SIZE - client->received, 0);
				if (r < 1) 
				{
					std::cout << " client " << connection.get_client_address(client)<< "disconnected"<< std::endl;
					connection.drop_client(&client_list, client);
				} 
				else 
				{
					client->received += r;
					client->request[client->received] = 0;
					char *q = strstr(client->request, "\r\n\r\n");
					if (q) 
					{
						*q = 0;
						// if (strncmp("GET /", client->request, 5)) 
						// {
						// 	connection.send_400(&client_list, client);
						// } 
						
						std::string decodedFilename;
						const char *request = client->request;
						// Parse the HTTP request
						std::istringstream requestStream(request);
						std::string method, path, version;
						requestStream >> method >> path >> version;
						// Trim leading '/' from the path
						path = path.substr(1);
						std::cout << "Method: " << method << std::endl;
						if (method == "POST") 
						{
							
							std::cout << "POST request" << std::endl;
							connection.postmethod(request, &client_list, client);
						}
						else if (method == "DELETE")
						{
						std::string filename;
						size_t lastSlash = path.find_last_of("/");
						if (lastSlash != std::string::npos) {
							filename = path.substr(lastSlash + 1);
						} else {
							filename = path;
						}
							std::cout << "DELETE request" << std::endl;
							std::cout << "Filename: " << filename << std::endl;
							connection.deletemethod(filename, &client_list, client);
						}
						else if (method == "GET") 
						{
							std::cout <<" Path is :" << path << std::endl;
							if (path == "" || path == "index.html") 
							{
								// Serve the HTML page
								connection.serve_resource(&client_list, client, path);
							}
							else if (path == "root")
							{
								connection.rootupdated = "";
								connection.serve_resource(&client_list, client, path);
							}
							else if (path == "upload")
							{
								connection.serve_resource(&client_list, client,path);
							}
							else if (path == "")
							{
									connection.serve_resource(&client_list, client, path);
									connection.rootupdated = "";
							}
							else if (path.compare(0, 8, "download") == 0) 
							{
								std::cout << "Download requested" << std::endl;
								std::string filename = path.substr(14); // Extract filename from "/download?file=<filename>"
								decodedFilename =  Utils::urlDecode(filename);
								std::cout << "Decoded filename: " << decodedFilename << std::endl;
								if(Utils::isDirectory(connection.rootDirectory + "/" + decodedFilename))
								{
									std::cout << "Root directory: " << connection.rootDirectory << std::endl;
									connection.rootupdated = connection.rootDirectory + "/" + decodedFilename;
									std::cout << "Root directory updated: " << connection.rootupdated<< std::endl;
									connection.serve_resource(&client_list, client,path);
								}
								else
									connection.downloadFile(filename, &client_list, client);
							} 
							else 
							{
								// Serve requested file
								connection.serve_resource(&client_list, client, path);
							}
						
						}
					}
				}
			}
			client = next;
		}
	}
	std::cout << "\nClosing socket..." << std::endl;
	close(server);
	std::cout << "Finished." << std::endl;
	return 0;
}

