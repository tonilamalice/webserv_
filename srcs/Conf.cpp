#include "Conf.hpp"
#include "server.hpp"
#include <vector>

Config::Config(void) {
}

Config::Config(const Config &other) : servers(other.servers) {
}

Config::~Config(void) {
}

Config &Config::operator=(const Config &other) {
    (void) other;
    return *this;
}

std::map<size_t, Server> &Config::getServers(void) {
    return (this->servers);
}

std::ostream &operator<<(std::ostream &out, Config &config) {

    std::map<size_t, Server> servers = config.getServers();

    for (std::map<size_t, Server>::iterator it=servers.begin();  it !=  servers.end(); it++) {
        out << it->second << std::endl;
    }
    return (out);
}
