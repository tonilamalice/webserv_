#include "Loc.hpp"
#include <iostream>
#include <sys/types.h>

Location::Location(void) 
    :path(""), root(""), index(""), autoindex(UNDEFINED), cgiPath(""), uploadDir("") {
    this->setAutorizedMethods(false, false, false);
    this->redirect.first = 0;
    this->redirect.second = "";
}

Location::Location(const Location &other) 
    : path(other.path), root(other.root), index(other.index), 
    autoindex(other.autoindex), redirect(other.redirect), cgiPath(other.cgiPath), uploadDir(other.uploadDir) {
    this->setAutorizedMethods(other.getGetVal(), other.getPostVal(), other.getDelVal());
}

Location::~Location(void) {
}

Location &Location::operator=(const Location &other) {
    this->path = other.path;
    this->root = other.root;
    this->index = other.index;
    this->autoindex = other.autoindex;
    this->redirect = other.redirect;
    this->setAutorizedMethods(other.getGetVal(), other.getPostVal(), other.getDelVal());
    this->cgiPath = other.cgiPath;
    this->uploadDir = other.uploadDir;
    return *this;
}

std::string Location::getPath(void) const {
    return (this->path);
};

std::string Location::getRoot(void) const {
    return (this->root);
};

std::string Location::getIndex(void) const {
    return (this->index);
};

size_t Location::getAutoIndex(void) const {
    return (this->autoindex);
}

std::pair<size_t, std::string> Location::getRedirect(void) const {
    return (this->redirect);
}

bool Location::getGetVal(void) const {
    return (this->autorizedMethods[GET]);
}

bool Location::getPostVal(void) const {
    return (this->autorizedMethods[POST]);
}

bool Location::getDelVal(void) const {
    return (this->autorizedMethods[DELETE]);
}

std::string Location::getCGIPath(void) const{
    return (this->cgiPath);
}

std::string Location::getUploadDir(void) const{
    return (this->uploadDir);
}

void Location::setPath(std::string path) {
    this->path = path;
}

void Location::setRoot(std::string root) {
    this->root = root;
}

void Location::setIndex(std::string index) {
    this->index = index;
}

void Location::setAutoIndex(size_t val) {
    this->autoindex = val;
}

void Location::setRedirection(size_t code, std::string path) {
    this->redirect.first = code;
    this->redirect.second = path;
}

void Location::setGet(bool val) {
    this->autorizedMethods[GET] = val;
}

void Location::setPost(bool val) {
    this->autorizedMethods[POST] = val;
}

void Location::setDel(bool val) {
    this->autorizedMethods[DELETE] = val;
}

void Location::setAutorizedMethods(bool get, bool post, bool del) {
    this->autorizedMethods[GET] = get;
    this->autorizedMethods[POST] = post;
    this->autorizedMethods[DELETE] = del;
}

void Location::setCGIPath(std::string &cgi) {
    this->cgiPath = cgi;
}

void Location::setUploadDir(std::string &uploadDir) {
    this->uploadDir = uploadDir;
}

std::ostream &operator<<(std::ostream &out, const Location &loc) {

    out << "    ->Location" << std::endl;
    out << "    Path : " << loc.getPath() << std::endl;
    out << "    Root : " << loc.getRoot() << std::endl;
    out << "    Index : {" << loc.getIndex() << "}";
    out << "    Autoindex : {" << loc.getAutoIndex() << "}";
    out << "    Redir : {[" << loc.getRedirect().first << "] " << loc.getRedirect().second << "}" << std::endl;  
    out << "    GET : {" << loc.getGetVal() << "}";
    out << "    POST : {" << loc.getPostVal() << "}";
    out << "    DEL : {" << loc.getDelVal() << "}" << std::endl;
    out << "    CGI : " << loc.getCGIPath() << std::endl;
    out << "    UploadDir : " << loc.getUploadDir() << std::endl;
    
    return (out);
}
