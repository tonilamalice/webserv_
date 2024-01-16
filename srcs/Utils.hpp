#ifndef Utils_HPP
#define Utils_HPP
#include "SocketCreation.hpp"
#include "Client.hpp"

class Utils
{
private:
    std::string requestData_;
	std::string boundary_;
	// std::map<std::string, std::string> mime_types; to implement

public:
    Utils(const std::string &requestData, const std::string &boundary);
    // Function to parse multipart form data
    void parseMultipartFormData();
	static std::string urlDecode(std::string str);
	static bool isFile(const std::string& path);
	static bool isDirectory(const std::string& path);
	static const char* get_content_type(const char *path);
	static std::string getMimeType(const std::string& filePath);
	// void store_mime_types(); /--> to implement
};
#endif
