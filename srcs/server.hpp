#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <ctype.h>
#include <map>
#include <vector>
#include <sys/types.h>
#include <iostream>
#include "Loc.hpp"


class Server {
	private:
		std::string                     root;
		std::string                     ip;
		size_t                          port;
		size_t                          maxBodySize;
		std::string                     serverName;
		std::map<size_t, std::string>   errors;
		std::vector<Location>           locations;

	public:
		Server(void);
		Server(const Server &other);
		~Server(void);
		Server &operator=(const Server &other);
		
		void        setPortNumber(size_t port);
		void        setServerRoot(std::string root);
		void        setIpAddress(std::string ip);
		void        setServerName(std::string name);
		void        setMaxBodySize(size_t size);
		void        setLocation(Location &loc);
		void        setError(size_t errCode, std::string errPath);

		std::string                     getServerRoot(void) const;
		std::string                     getIpAddress(void)   const;
		size_t                          getPortNumber(void) const;
		std::string					    getPortString(void) const;
		size_t                          getMaxBodySize(void) const;
		std::string                     getServerName(void) const;
		std::map<size_t, std::string>   getErrors(void) const;
		std::vector<Location>           getLocations(void) const;

};

std::ostream &operator<<(std::ostream &out, const Server &pt);

#endif
