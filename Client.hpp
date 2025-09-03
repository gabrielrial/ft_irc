#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "lib_irc.hpp"

class Client {
public:
    Client();
    Client(const Client& other);
    Client& operator=(const Client& other);
    ~Client();

private:
    std::string _name_client;
    int         _socket_client;
    int         _current_channel;
};
std::ostream &operator<<(std::ostream &os, const Client &Client);

#endif