#include "server.hpp"
#include "Loc.hpp"
#include <cstddef>
#include <exception>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>
#include <sstream>

Server::Server(void) 
    :root(""), ip(""), port(0), maxBodySize(10000), serverName("") {
}

Server::Server(const Server &other) 
    : root(other.root), ip(other.ip), port(other.port), maxBodySize(other.getMaxBodySize()), 
    serverName(other.serverName), errors(other.errors) {
}

Server::~Server(void) {
}


Server &Server::operator=(const Server &other) {
    this->root = other.getServerRoot();
    this->serverName = other.getServerName();
    this->ip = other.getIpAddress();
    this->port = other.getPortNumber();
    this->maxBodySize = other.getMaxBodySize();
    this->errors = other.errors;
    return *this;
}

std::string Server::getPortString(void) const {
	std::string port;
	std::stringstream ss;
	ss << this->port;
	ss >> port;
	return (port);
}

void Server::setServerRoot(std::string rootPath) {
    struct stat dir_stat;
    if (stat(rootPath.c_str(), &dir_stat) == 0) {
        if (S_ISDIR(dir_stat.st_mode)) {
            this->root = rootPath;
        }
        else {
            std::cout << "Not a directory" << std::endl;
        } 
    }
    else { 
        std::cout<< "Directory does not exist" << std::endl;
    }
}

void Server::setIpAddress(std::string ip) {
    this->ip = ip;
}

void Server::setPortNumber(size_t port) {
    this->port = port;
}

void Server::setMaxBodySize(size_t size) {
    this->maxBodySize = size;
}

void Server::setServerName(std::string name) {
    this->serverName = name;
}

void Server::setError(size_t errCode, std::string errPath) {
    if (errors.count(errCode) == 1) {
        std::cout << "Error code already exists" << std::endl;
    }
    this->errors[errCode] = errPath;
}
void Server::setLocation(Location &loc) {
    this->locations.push_back(loc);
}
std::vector<Location> Server::getLocations(void) const {
    return (this->locations);
}

std::string Server::getServerRoot(void) const {
    return (this->root);
}

std::string Server::getIpAddress(void) const {
    return (this->ip);
}

std::size_t Server::getPortNumber(void) const {
    return (this->port);
}

std::size_t Server::getMaxBodySize(void) const {
    return (this->maxBodySize);
}

std::string Server::getServerName(void) const {
    return (this->serverName);
}

std::map<size_t, std::string> Server::getErrors(void) const {
    return (this->errors);
}



std::ostream &operator<<(std::ostream &out, const Server &serv) {
    
    out << "Server RootPath : " << serv.getServerRoot() << std::endl;
    out << "Server Name : " << serv.getServerName() << std::endl;
    out << "IP : " << serv.getIpAddress() << std::endl;
    out << "Port : " << serv.getPortNumber() << std::endl;
    out << "Max Body Size : " << serv.getMaxBodySize() << std::endl;
    
    std::map<size_t, std::string> errors = serv.getErrors();
    for (std::map<size_t, std::string>::iterator it = errors.begin(); 
            it != errors.end(); it++) {
        out << "Error : [" << it->first << "]" << " : " << it->second << std::endl; 
    }
    
    std::vector<Location> loc = serv.getLocations();
    for (size_t i = 0; i < loc.size(); i++) {
        out << loc[i] << std::endl;
    }

    return (out);
}
