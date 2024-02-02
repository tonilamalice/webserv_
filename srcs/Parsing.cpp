#include "Parsing.hpp"
#include "Loc.hpp"
#include <stack>

void parseConfiguration(std::string configFile, Config &conf) {

    std::ifstream file(configFile.c_str());

    if (file.fail() || !hasConfExtension(configFile)) {
        std::cout << "Error opening file" << std::endl;
    }

    std::string line;
    std::streampos nextPos = 0;
    bool createdServer = false;

    while (std::getline(file.seekg(nextPos), line)) {
        std::string infoBuffer;
        if (line.find("server") != std::string::npos) {
            nextPos = getInstructionBlock(file, infoBuffer);
            addServer(infoBuffer, conf);
            createdServer = true;
        }
        else {
            if (!isLineEmpty(line)) {
                std::cout << "Unvalid directive" << std::endl;
            }
            nextPos += 1;
        }
    }
    if (createdServer == false) {
       std::cout << "No server block found" << std::endl;
    }
}
 
bool hasConfExtension(const std::string& filename) {
    if (filename.length() < 5) 
        return (false);
    return (filename.substr(filename.length() - 5) == ".conf");
}

std::streampos  getInstructionBlock(std::ifstream &file, std::string &outputBuffer) {

    std::stack<char>    bracketStack;
    std::string         line;
    
    bracketStack.push('{');

    while (std::getline(file, line)) {
        if (line.find('{') != std::string::npos) {
            bracketStack.push('{');
        }
        else if (line.find('}') != std::string::npos) {
            if (!bracketStack.empty()) {
                bracketStack.pop(); 
                if (bracketStack.empty()) {
                    break;
                }
            }
            else {
                std::cout<< "Unterminated block" << std::endl;
            }
        }
        outputBuffer += (line + '\n');
    }
    if (!bracketStack.empty()) {
        std::cout << "Unterminated block" << std::endl;
    }
    return(file.tellg());
}

void getLocationBlock(std::stringstream &ss, std::string &outputBuffer) {

    std::stack<char>    bracketStack;
    std::string         line;
    
    bracketStack.push('{');
    outputBuffer += '\n';

    while (std::getline(ss, line)) {
        line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
        if (line.find('{') != std::string::npos) {
            bracketStack.push('{');
        }
        else if (line.find('}') != std::string::npos) {
            if (!bracketStack.empty()) {
                bracketStack.pop(); 
                if (bracketStack.empty()) {
                    break;
                }
            }
            else {
               std::cout << "Unterminated block" << std::endl;
            }
        }
        outputBuffer += (line + '\n');
    }
    if (!bracketStack.empty()) {
        std::cout << "Unterminated block" << std::endl;
    }
}
 
int nextMatchingBracket(std::string input, std::string &outputBuffer, size_t startPos) {

    std::stack<char>    bracketStack;
    size_t              bracketStart = 0;

    for (size_t i = startPos; i < input.length(); i++) {
        if (input[i] == '{') {
            if (bracketStart == 0) {
                bracketStart = i;
            }
            bracketStack.push('{');
        }
        else if (input[i] == '}') {
            if (bracketStack.empty()) {
               std::cout << "Unmatched closing bracket" << std::endl;
            }
            bracketStack.pop();
            if (bracketStack.empty()) {
                outputBuffer = input.substr(bracketStart + 1, i - bracketStart - 1);
                return (i + 1);
            }
        }
    }
    std::cout << "Unterminated block" << std::endl;
	return (-1);
}

int findMatchingValue(std::string inputString, std::string directive, std::string &outputBuffer) {
    std::size_t valuePos = directive.length();
    std::size_t endPos = inputString.find(";", valuePos);
    if (endPos != std::string::npos) {
        outputBuffer = inputString.substr(valuePos, endPos - valuePos);
        if (inputString.length() != outputBuffer.length() + directive.length() + 1) {
            std::cout << "Unvalid directive" << std::endl;
        }
        return (endPos + 1);
    }
    else {
        std::cout << "Unvalid directive" << std::endl;
    }
    return (-1);
}

int matchDirective(std::string line, std::string *directives, size_t nbrDirectives) {
    
    for (size_t directiveID = 0; directiveID < nbrDirectives; directiveID++) {
        if (line.find(directives[directiveID]) == 0) {
            return (directiveID);
        }
    }
    return (-1);
}

void addServer(const std::string infoBuffer, Config& conf) {
	Server serv;
	std::string directives[6] = {"listen", "server_name", "error_page", "location", "max_body_size", "root"};

	std::stringstream ss(infoBuffer);
	std::string line;

	while (std::getline(ss, line)) {
		std::string value = "";
		line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
		if (line == "" || line == "{" || line == "}") {
			continue;
		} else {
			int directiveID = matchDirective(line, directives, 6);

			switch (directiveID) {
				case 0:
					findMatchingValue(line, directives[directiveID], value);
					addIpPort(value, serv);
					break;

				case 1:
					findMatchingValue(line, directives[directiveID], value);
					serv.setServerName(value);
					break;

				case 2:
					findMatchingValue(line, directives[directiveID], value);
					setErrorPages(value, serv);
					break;

				case 3:
					if (serv.getServerRoot().empty()) {
						std::cout << "Server root not defined" << std::endl;
					}
					getLocationBlock(ss, line);
					createLocationInfo(line, serv);
					break;

				case 4:
					findMatchingValue(line, directives[directiveID], value);
					setMaxBodySize(value, serv);
					break;

				case 5:
					findMatchingValue(line, directives[directiveID], value);
					if (value.empty() || value[0] != '/') {
						std::cout << "Root is not absolute" << std::endl;
					}
					serv.setServerRoot(value);
					break;

				default:
					std::cout << "Invalid directive" << std::endl;
			}
		}
	}

	if (conf.getServers().count(serv.getPortNumber()) == 1) {
		std::cout << "Port already exists" << std::endl;
	}
	if (serv.getServerRoot().empty() || serv.getIpAddress().empty()) {
		std::cout << "Server root or IP not defined" << std::endl;
	}
	if (serv.getLocations().empty()) {
		std::cout << "No location defined" << std::endl;
	}
	conf.getServers()[serv.getPortNumber()] = serv;
	std::cout << serv << std::endl;
}

void setMaxBodySize(const std::string value, Server& serv) {
	if (!isNumericString(value)) {
		std::cout << "Invalid value" << std::endl;
	}
	std::stringstream ss(value);
	size_t convVal;
	ss >> convVal;
	if (ss.fail()) {
		std::cout << "Invalid value" << std::endl;
	}
	serv.setMaxBodySize(convVal);
}

void setErrorPages(const std::string infoBuffer, Server& serv) {
	std::string errCode;
	std::string errPath;
	size_t convErrCode;

	errCode = infoBuffer.substr(0, 3);
	errPath = infoBuffer.substr(3, infoBuffer.length());

	if (!isNumericString(errCode))
		std::cout << "Invalid value" << std::endl;

	std::stringstream ss(errCode);
	ss >> convErrCode;

	serv.setError(convErrCode, errPath);
}

void addIpPort(const std::string values, Server& serv) {
	std::size_t sep = values.find(":");
	if (sep != std::string::npos) {
		std::string ip = values.substr(0, sep);
		serv.setIpAddress(ip);

		std::string port = values.substr(sep + 1, values.length());
		std::stringstream ss(port);
		if (!isNumericString(port)) {
			std::cout << "Invalid value" << std::endl;
		}

		size_t convPort;

		ss >> convPort;
		if (ss.fail()) {
			std::cout << "Invalid value" << std::endl;
		} else {
			serv.setPortNumber(convPort);
		}
	} else {
		std::cout << "Invalid value" << std::endl;
	}
}

std::string getLocationPath(const std::string& infoBuffer, size_t startPos) {
	std::string path;
	size_t endPos = infoBuffer.find("{", startPos);
	if (endPos == std::string::npos) {
		std::cout << "Unterminated block" << std::endl;
	}
	path = infoBuffer.substr(startPos, endPos - startPos);
	if (path.empty() || path[0] != '/') {
		std::cout << "Location path is not absolute" << std::endl;
	}
	return path;
}

void createLocationInfo(const std::string inputBuffer, Server& serv) {
	Location loc;
	std::string directives[8] = {"location", "root", "accept", "cgi", "index", "autoindex", "redirect", "upload_dir"};
	std::stringstream ss(inputBuffer);
	std::string line;
	bool definedActionOnLoading = false;
	std::string servRoot = serv.getServerRoot();

	while (std::getline(ss, line)) {
		std::string value = "";
		line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
		if (line == "" || line == "{" || line == "}") {
			continue;
		} else {
			int directiveID = matchDirective(line, directives, 8);

			switch (directiveID) {
				case 0:
					loc.setPath(getLocationPath(line, 8));
					break;

				case 1:
					findMatchingValue(line, directives[directiveID], value);
					if (value.empty() || value[0] != '/') {
						std::cout << "Root is not absolute" << std::endl;
					}
					loc.setRoot(value);
					break;

				case 2:
					findMatchingValue(line, directives[directiveID], value);
					addAllowedMethods(value, loc);
					break;

				case 3:
					findMatchingValue(line, directives[directiveID], value);
					loc.setCGIPath(value);
					loc.setUploadDir(servRoot);
					break;

				case 4:
					if (definedActionOnLoading == false) {
						findMatchingValue(line, directives[directiveID], value);
						loc.setIndex(value);
						definedActionOnLoading = true;
					} else
						std::cout << "Conflicting instruction" << std::endl;
					break;

				case 5:
					if (definedActionOnLoading == false) {
						findMatchingValue(line, directives[directiveID], value);
						if (value == "true")
							loc.setAutoIndex(true);
						else if (value == "false")
							loc.setAutoIndex(false);
						else
							std::cout << "Invalid value" << std::endl;
						definedActionOnLoading = true;
					} else
						std::cout << "Conflicting instruction" << std::endl;
					break;

				case 6:
					if (definedActionOnLoading == false) {
						findMatchingValue(line, directives[directiveID], value);
						setRedirection(value, loc);
						definedActionOnLoading = true;
					} else
						std::cout << "Conflicting instruction" << std::endl;
					break;

				case 7:
					if (loc.getCGIPath().empty()) {
						std::cout << "CGI path not defined" << std::endl;
					}
					findMatchingValue(line, directives[directiveID], value);
					if (value.empty()) {
						std::cout << "Empty upload dir" << std::endl;
					}
					value = servRoot + "/" + value;
					loc.setUploadDir(value);
					break;

				default:
					std::cout << "Invalid directive" << std::endl;
			}
		}
	}
	serv.setLocation(loc);
}

void checkUploadDir(Server& serv, Location& loc) {
	std::cout << serv.getServerRoot() << std::endl;
	std::cout << loc.getUploadDir() << std::endl;
}

void setRedirection(const std::string inputBuffer, Location& loc) {
	std::string redirCode = inputBuffer.substr(0, 3);
	std::string redirPath = inputBuffer.substr(3, inputBuffer.length());

	size_t convCode;
	if (!isNumericString(redirCode))
		std::cout << "Invalid value" << std::endl;

	std::stringstream ss(redirCode);
	ss >> convCode;
	if (ss.fail()) {
		std::cout << "Invalid value" << std::endl;
	}
	loc.setRedirection(convCode, redirPath);
}

void addAllowedMethods(const std::string infoBuffer, Location& loc) {
	int methodID;

	methodID = matchRequestMethod(infoBuffer);
	switch (methodID) {
		case GET:
			loc.setGet(true);
			break;
		case POST:
			loc.setPost(true);
			break;
		case DELETE:
			loc.setDel(true);
			break;
		default:
			std::cout << "Invalid method" << std::endl;
	}
}

int matchRequestMethod(const std::string method) {
	size_t retVal = -1;
    std::string methods[3] = {"GET", "POST", "DELETE"};

	    for (size_t methodID = 0; methodID < 3; methodID++) {
        if (method == methods[methodID]) {
            retVal = methodID;
        }
    } 
    return (retVal);
}

bool isLineEmpty(const std::string str) {
	for (size_t i = 0; i < str.length(); i++) {
		if (!isspace(str[i])) {
			return false;
		}
	}
	return true;
}

bool isNumericString(const std::string& input) {
	for (size_t i = 0; i < input.size(); i++) {
		if (!std::isdigit(input[i]))
			return false;
	}
	return true;
}
