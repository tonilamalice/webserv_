#include "Client.hpp"


Client::Client()
{
	rootDirectory = "./uploads";
	this->rootupdated = "";
}

Client::~Client()
{
}

struct Client::client_info *Client::get_client(struct client_info **client_list, int s)
{
	struct client_info *ci = *client_list;
	while (ci)
	{
		if (ci->socket == s)
			return (ci);
		ci = ci->next;
	}
	if (ci) return ci;
	struct client_info *n =
		(struct client_info*) calloc(1, sizeof(struct client_info));

	if (!n) {
		fprintf(stderr, "Out of memory.\n");
		exit(1);
	}

	n->address_length = sizeof(n->address);
	n->next = *client_list;
	*client_list = n;
	return n;
}
void Client::drop_client(struct client_info **client_list, struct client_info *client)
{
	close(client->socket);
	struct client_info **p = client_list;

	while (*p)
	{
		if (*p == client)
		{
			*p = client->next;
			free(client);
			return;
		}
		p = &(*p)->next;
	}
	fprintf(stderr, "drop_client not found.\n");
	exit(1);
}
const char *Client::get_client_address(struct client_info *ci)
{
	getnameinfo((struct sockaddr*)&ci->address,
				ci->address_length,
				ci->address_buffer,
				sizeof(ci->address_buffer),
				0, 0,
				NI_NUMERICHOST);
	return ci->address_buffer;
}
fd_set Client::wait_on_clients(struct client_info **client_list, int server)
{
	fd_set reads;
	FD_ZERO(&reads);
	FD_SET(server, &reads);
	int max_socket = server;
	struct client_info *ci = *client_list;
	while (ci)
	{
		FD_SET(ci->socket, &reads);
		if (ci->socket > max_socket)
			max_socket = ci->socket;
		ci = ci->next;
	}
	if (select(max_socket + 1, &reads, 0, 0, 0) < 0)
	{
		fprintf(stderr, "select() failed. (%d)\n", errno);
		exit(1);
	}
	return reads;
}

void Client::send_400(struct client_info **client_list, struct client_info *client)
{
	const char *c400 = "HTTP/1.1 400 Bad Request\r\n"
		"Connection: close\r\n"
		"Content-Length: 11\r\n\r\nBad Request";
	send(client->socket, c400, strlen(c400), 0);
	drop_client(client_list, client);
}

void Client::send_404(struct client_info **client_list, struct client_info *client)
{
	const char *c404 = "HTTP/1.1 404 Not Found\r\n"
		"Connection: close\r\n"
		"Content-Length: 9\r\n\r\nNot Found";
	send(client->socket, c404, strlen(c404), 0);
	drop_client(client_list, client);
}
void Client::send_500(struct client_info **client_list, struct client_info *client)
{
	const char *c500 = "HTTP/1.1 500 Internal Server Error\r\n"
		"Connection: close\r\n"
		"Content-Length: 21\r\n\r\nInternal Server Error";
	send(client->socket, c500, strlen(c500), 0);
	drop_client(client_list, client);
}

void Client::send_501(struct client_info **client_list, struct client_info *client)
{
	const char *c501 = "HTTP/1.1 501 Not Implemented\r\n"
		"Connection: close\r\n"
		"Content-Length: 15\r\n\r\nNot Implemented";
	send(client->socket, c501, strlen(c501), 0);
	drop_client(client_list, client);
}


void::Client::getFilesInDirectory(std::vector<std::string> &files){
	DIR* dir;
    struct dirent* entry;
	std::string filePath;
		if (rootupdated == "")
        	filePath = rootDirectory;
		else
			filePath = rootupdated;
		
    if ((dir = opendir(filePath.c_str())) != NULL) {
        while ((entry = readdir(dir)) != NULL) {
            // Skip "." and ".." entries
            if (std::string(entry->d_name) != "." && std::string(entry->d_name) != "..") {
                files.push_back(entry->d_name);
            }
        }
        closedir(dir);
    }
}
// std::ostringstream Client::sendFileList(){

// 	std::vector<std::string> files;
// 	getFilesInDirectory(files);

// 	std::ostringstream oss;
//     oss << "HTTP/1.1 200 OK\r\n";
//     oss << "Content-Type: text/html\r\n\r\n";
//     oss << "<!DOCTYPE html>\n";
//     oss << "<html lang=\"en\">\n";
//     oss << "<head>\n";
//     oss << "    <meta charset=\"UTF-8\">\n";
//     oss << "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
//     oss << "    <title>Movies of the month</title>\n";
//     oss << "</head>\n";
//     oss << "<body>\n";
//     oss << "    <h3><button onclick=\"window.location.href = '/root'\">AAA Movies Root</button></h3>\n";
// 	oss << "    <h1><button onclick=\"window.location.href = 'upload'\">Upload files</button></h1>\n";
//     oss << "    <ul>\n";

//     // Add a button for each file
//     for (std::vector<std::string>::const_iterator it = files.begin(); it != files.end(); ++it) {
//         oss << "        <li><button onclick=\"window.location.href = '/download?file=" << *it << "'\">" << *it << "</button></li>\n";
//     }

//     oss << "    </ul>\n";
//     oss << "</body>\n";
//     oss << "</html>\n";

// 	return oss;


// }
void Client::serve_resource(struct client_info **client_list, struct client_info *client, std::string path2)
{
	const char *path = path2.c_str();

	if (strcmp(path, "") == 0) path = "index.html";
	else if (strcmp(path, "root") == 0) path = "root.html";
	else if (strcmp(path, "upload") == 0) path = "upload.html";
	else 
		path = "index.html";
	if (strlen(path) > 100) {
		send_400(client_list, client);
		return;
	}

	if (strstr(path, "..")) {
		send_404(client_list, client);
		return;
	}

	std::ifstream file(path, std::ios::binary);

	if (!file) {
		send_404(client_list, client);
		std::cout << "404" << std::endl;
		return;
	}

	file.seekg(0, std::ios::end);
	// size_t content_length = file.tellg();
	file.seekg(0, std::ios::beg);

	// const char *content_type = Utils::get_content_type(path);
	// std::ostringstream oss;

	std::vector<std::string> files;
	getFilesInDirectory(files);

	std::ostringstream oss;
    oss << "HTTP/1.1 200 OK\r\n";
    oss << "Content-Type: text/html\r\n\r\n";
    oss << "<!DOCTYPE html>\n";
    oss << "<html lang=\"en\">\n";
    oss << "<head>\n";
    // oss << "    <meta charset=\"UTF-8\">\n";
    // oss << "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
    // oss << "    <title>Movies of the month</title>\n";
    // oss << "</head>\n";
    // oss << "<body>\n";
    // oss << "    <h3><button onclick=\"window.location.href = '/root'\">AAA Movies Root</button></h3>\n";
	// oss << "    <h1><button onclick=\"window.location.href = 'upload'\">Upload files</button></h1>\n";
    // oss << "    <ul>\n";

    // // Add a button for each file
    // for (std::vector<std::string>::const_iterator it = files.begin(); it != files.end(); ++it) {
    //     oss << "        <li><button onclick=\"window.location.href = '/download?file=" << *it << "'\">" << *it << "</button></li>\n";
    // }

    // oss << "    </ul>\n";
    // oss << "</body>\n";
    // oss << "</html>\n";
	//  oss = sendFileList();

	std::string response = oss.str();
	char buffer[1024];
	send(client->socket, response.c_str(), response.size(), 0);
	while (file.read(buffer, sizeof(buffer)).gcount() > 0) {
		send(client->socket, buffer, file.gcount(), 0);
	}

	file.close();
	drop_client(client_list, client);
}

void Client::downloadFile(const std::string& filename,struct client_info **client_list, struct client_info *client) 
{
	std::string filePath;
	std::string decodedFilename = Utils::urlDecode(filename);

	
	if (rootupdated == "")
		filePath = rootDirectory + "/" + decodedFilename;
		
	else
		filePath = rootupdated + "/" + decodedFilename;
	std::cout << "File path: " << filePath << std::endl;
	// Open the requested file
	std::ifstream file(filePath.c_str(), std::ios::binary);

	// Check if the file is opened successfully
		if (file.is_open()) 
		{
			// Build the HTTP response headers
			std::ostringstream responseHeaders;
			responseHeaders << "HTTP/1.1 200 OK\r\n";
			responseHeaders << "Content-Type: " << Utils::getMimeType(filePath) << "\r\n";
			responseHeaders << "Content-Disposition: attachment; filename=\"" << decodedFilename << "\"\r\n";

			// Get the file size
			file.seekg(0, std::ios::end);
			std::streampos fileSize = file.tellg();
			file.seekg(0, std::ios::beg);

			// Add Content-Length header
			responseHeaders << "Content-Length: " << fileSize << "\r\n\r\n";

			// Send the response headers
			
			write(client->socket, responseHeaders.str().c_str(), responseHeaders.str().length());

			// Stream the file in chunks
			const std::streamsize bufferSize = 8192; // Adjust the buffer size as needed
			char buffer[bufferSize];

			while (!file.eof()) 
				{
					file.read(buffer, sizeof(buffer));
					ssize_t result = write(client->socket, buffer, file.gcount());
					if (result == -1) 
					{
						std::cerr << "Error writing to socket." << std::endl;
						break;
					}
				}

			// Close the file
			file.close();
			drop_client(client_list, client);
    	} 
		else {
        // HTTP 404 Not Found - Respond with an error message
        send_404(client_list, client);
    }
}

void Client::postmethod(const char* request, struct client_info **client_list, struct client_info *client)
{
	   std::istringstream requestStream(request);
        std::string line;
        std::string contentType;
        while (std::getline(requestStream, line) && !line.empty()) 
			{
				// Process headers if needed
				std::cout << "line is: " << line << std::endl;
				if (line.find("Content-Type:") != std::string::npos) 
					{
						contentType = line.substr(14);
						std::cout << "Content-Type is: " << contentType << std::endl;
						break;
					}
			}
        // Extract the boundary string from the Content-Type header
        size_t boundaryPos = contentType.find("boundary=");
        std::string boundary = contentType.substr(boundaryPos + 9);

        // Read the content length to know the size of the incoming data
        std::string contentLengthHeader;
        while (std::getline(requestStream, line) && !line.empty()) 
		{
            if (line.find("Content-Length:") != std::string::npos) 
				{
					contentLengthHeader = line;
					break;
				}
        }

        size_t contentLength = 0;
        if (!contentLengthHeader.empty()) {
            contentLength = std::stoi(contentLengthHeader.substr(16));
        }

        std::cout << "------>Content length: " << contentLength << std::endl;

        // Read the request body
        std::vector<char> requestBody(contentLength, 0);
        size_t bytesRead = 0;
        while (bytesRead < contentLength) {
            int readResult = read(client->socket, requestBody.data() + bytesRead, contentLength - bytesRead);
            if (readResult == -1) {
                // Handle read error
                std::cerr << "Error reading request body." << std::endl;
                // sendErrorResponse(500, "Internal Server Error");
                close(client->socket);
                return;
            } else if (readResult == 0) {
                // Connection closed prematurely
                std::cerr << "Error: Connection closed prematurely." << std::endl;
                // sendErrorResponse(400, "Bad Request");
                close(client->socket);
                return;
            }
            bytesRead += readResult;
        }

        // Now, requestBody contains the POST data
        std::string requestBodyStr(requestBody.begin(), requestBody.end());

        std::cout << "------>Request data: " << requestBodyStr << std::endl;
        std::cout << "------>Boundary: " << boundary << std::endl;

        Utils parsing(requestBodyStr, boundary);
        parsing.parseMultipartFormData();
		drop_client(client_list, client);
}