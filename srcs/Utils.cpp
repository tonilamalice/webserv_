#include "Utils.hpp"

Utils::Utils(const std::string &requestData, const std::string &boundary)
        : requestData_(requestData), boundary_(boundary) 
    {
        // Extract boundary from Content-Type header
        size_t contentTypePos = requestData_.find("Content-Type:");
        if (contentTypePos != std::string::npos)
        {
            size_t boundaryPos = requestData_.find("boundary=", contentTypePos);
            if (boundaryPos != std::string::npos)
            {
                size_t boundaryEnd = requestData_.find("\r\n", boundaryPos);
                if (boundaryEnd != std::string::npos)
                {
                    boundary_ = requestData_.substr(boundaryPos + 9, boundaryEnd - (boundaryPos + 9));
                }
            }
        }
    }
    // Function to parse multipart form data
    void Utils::parseMultipartFormData() {
        // Find the boundary in the request data
		std::cout << "request data: " << requestData_ << std::endl;
        size_t boundaryPos = requestData_.find("boundary=");
        // if (boundaryPos == std::string::npos) {
        //     std::cerr << "Error: Boundary not found in request data." << std::endl;
        //     return;
        // }

        std::string boundary = requestData_.substr(boundaryPos + 9);

        // Split the request data into parts using the boundary
        size_t startPos = requestData_.find("\r\n\r\n") + 4;
        size_t endPos = requestData_.find("--" + boundary, startPos);

        // if (startPos == std::string::npos || endPos == std::string::npos) {
        //     std::cerr << "Error: Invalid request data format." << std::endl;
        //     return;
        // }

        std::string fileData = requestData_.substr(startPos, endPos - startPos - 2);

        // Extract filename from the Content-Disposition header
        size_t filenamePos = requestData_.find("filename=\"");
        size_t filenameEndPos = requestData_.find("\"", filenamePos + 10);

        std::string filename = requestData_.substr(filenamePos + 10, filenameEndPos - filenamePos - 10);

    // Save file data to a file
        std::ofstream outFile(filename.c_str(), std::ios::binary);
        if (!outFile) {
            std::cerr << "Error opening file for writing: " << filename << std::endl;
            return;
        }

        outFile.write(fileData.c_str(), fileData.size());
        outFile.close();

        std::cout << "File saved as: " << filename << std::endl;
    }

	std::string Utils::urlDecode(std::string str)
{
	
    std::string ret;
    char ch;
    int i, ii;
    for (i=0; i<(int)str.length(); i++) {
        if(str[i] != '%'){
            if(str[i] == '+')
                ret += ' ';
            else
                ret += str[i];
        }else{
            sscanf(str.substr(i + 1, 2).c_str(), "%x", &ii);
            ch = static_cast<char>(ii);
            ret += ch;
            i = i + 2;
        }
    }
    return ret;
}
bool Utils::isDirectory(const std::string& path) {
    struct stat fileStat;
    if (stat(path.c_str(), &fileStat) == 0) {
        return S_ISDIR(fileStat.st_mode) != 0;
    }
    return false; // Error occurred, consider it as not a directory
}
bool Utils::isFile(const std::string& path) {
    struct stat fileStat;
    if (stat(path.c_str(), &fileStat) == 0) {
        return S_ISREG(fileStat.st_mode) != 0;
    }
    return false; // Error occurred, consider it as not a file
}
const char* Utils::get_content_type(const char *path)
{
	const char *last_period = strrchr(path, '.');
	if (last_period) {
		if (strcmp(last_period, ".css") == 0) return "text/css";
		if (strcmp(last_period, ".csv") == 0) return "text/csv";
		if (strcmp(last_period, ".gif") == 0) return "image/gif";
		if (strcmp(last_period, ".htm") == 0) return "text/html";
		if (strcmp(last_period, ".html") == 0) return "text/html";
		if (strcmp(last_period, ".ico") == 0) return "image/x-icon";
		if (strcmp(last_period, ".jpeg") == 0) return "image/jpeg";
		if (strcmp(last_period, ".jpg") == 0) return "image/jpeg";
		if (strcmp(last_period, ".js") == 0) return "application/javascript";
		if (strcmp(last_period, ".json") == 0) return "application/json";
		if (strcmp(last_period, ".png") == 0) return "image/png";
		if (strcmp(last_period, ".pdf") == 0) return "application/pdf";
		if (strcmp(last_period, ".svg") == 0) return "image/svg+xml";
		if (strcmp(last_period, ".txt") == 0) return "text/plain";
	}
	return "application/octet-stream";
}
std::string Utils::getMimeType(const std::string& filePath) {
        // Determine the MIME type based on the file extension (simple example)
        size_t dotPos = filePath.find_last_of('.');
        if (dotPos != std::string::npos) {
            std::string extension = filePath.substr(dotPos + 1);
            if (extension == "mp4") {
                return "video/mp4";
			}
			if (extension == "png") {
				return "image/png";
            }
			if (extension == "jpg") {
				return "image/jpg";
			}
			if (extension == "jpeg") {
				return "image/jpeg";
			}
			if (extension == "gif") {
				return "image/gif";
			}
            // Add more cases for other file types as needed
        }
        return "application/octet-stream"; // Default to generic binary type
    }

// 	void Client::store_mime_types() --> to implement and replace 2 above functions

// {
// 	// Open the mime.types file
// 	std::ifstream file("conf/mime.types");
// 	if (file.is_open()) {
// 		std::string line;
// 		while (std::getline(file, line)) {
// 			// Skip empty lines and comments
// 			if (line.empty() || line[0] == '#') {
// 				continue;
// 			}

// 			// Split the line into file extension and mime type
// 			std::size_t pos = line.find(',');
// 			if (pos != std::string::npos) {
// 				std::string extension = line.substr(0, pos);
// 				std::string mime_type = line.substr(pos + 1);

// 				// Add the mapping to the mime_types map
// 				mime_types[extension] = mime_type;
// 			}
// 		}
// 		// Close the file
// 		file.close();
// 	}
// }
