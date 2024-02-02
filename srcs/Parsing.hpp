#ifndef PARSING_HPP
#define PARSING_HPP
#include "Client.hpp"
#include "Loc.hpp"
#include "Conf.hpp"
#include "server.hpp"

void parseConfiguration(std::string configFile, Config &conf);
int matchDirective(std::string line, std::string *directives, size_t nbrDirectives);
void getLocationBlock(std::stringstream &ss, std::string &outputBuffer);
void setMaxBodySize(const std::string value, Server& serv);
void addIpPort(const std::string values, Server& serv);
bool hasConfExtension(const std::string& filename);
std::streampos  getInstructionBlock(std::ifstream &file, std::string &outputBuffer);
void setErrorPages(const std::string infoBuffer, Server& serv) ;
int findMatchingValue(std::string inputString, std::string directive, std::string &outputBuffer);
void addServer(const std::string infoBuffer, Config& conf);
void checkUploadDir(Server& serv, Location& loc);
void createLocationInfo(const std::string inputBuffer, Server& serv);
std::string getLocationPath(const std::string& infoBuffer, size_t startPos);
void setRedirection(const std::string inputBuffer, Location& loc);
bool isNumericString(const std::string& input);
int matchRequestMethod(const std::string method);
void addAllowedMethods(const std::string infoBuffer, Location& loc);
int nextMatchingBracket(std::string input, std::string &outputBuffer, size_t startPos) ;
bool isLineEmpty(const std::string str) ;
#endif 