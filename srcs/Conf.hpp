#ifndef CONF_HPP
#define CONF_HPP
#include <cstddef>
#include <vector>
#include "server.hpp"

class Config {
    private:
        std::map<size_t, Server> servers;

    public:
        Config(void);
        Config(const Config &other);
        ~Config(void);
        Config &operator=(const Config &other); 

        std::map<size_t, Server> &getServers(void);
};

std::ostream &operator<<(std::ostream &out, Config &pt);

#endif
