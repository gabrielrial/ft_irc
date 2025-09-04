#include "Client.hpp"
#include <unistd.h>   // close()
#include <cstring>    // memset
#include <iostream>   // std::cerr

// Constructor
Client::Client(int socket) : _socket(socket), _name(""), _hostname("") {
    // Inicializar _pollClient
    memset(&_pollClient, 0, sizeof(_pollClient));
    _pollClient.fd = _socket;
    _pollClient.events = POLLIN;  // solo nos interesa leer
    _pollClient.revents = 0;
}

// Destructor
Client::~Client() {
    if (_socket != -1) {
        close(_socket);
        _socket = -1;
    }
}

// Get socket descriptor
int Client::getSocket() const {
    return _socket;
}

// Set client nickname
void Client::setName(const std::string &name) {
    _name = name;
}

// Get client nickname
std::string Client::getName() const {
    return _name;
}

// Get pollfd pointer
pollfd* Client::getPollFD() {
    return &_pollClient;
}
