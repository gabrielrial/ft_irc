#include "Client.hpp"

Client::Client(int fd, sockaddr_in &addr) : _socket_fd(fd), _channel_amount(0),
        _server_addr(addr), _nickname("*"), _username(""), channels(0)
{
    char ip[INET_ADDRSTRLEN]; // standard IPv4 len
    inet_ntop(AF_INET, &_server_addr.sin_addr, ip, sizeof(ip)); // converts binary IP address to readable, stored in ip
    _ip = std::string(ip);
    _port = ntohs(addr.sin_port);
}

Client::~Client()
{
    if (_socket_fd > 0)
        close(_socket_fd);
    return ;
}
//              getters
int Client::getFd() const
{
    return _socket_fd;
}

int Client::getPort() const
{
    return _port;
}

std::string Client::getIP() const
{
    return _ip;
}

std::string Client::getNickname() const
{
    return _nickname;
}

std::string Client::getUsername() const
{
    return _username;
}

int Client::get_channel_amt()
{
    return this->_channel_amount;
}

//              setters
void    Client::setNickname(const std::string &nick)
{
    this->_nickname = nick;
}

void    Client::setUsername(const std::string &user)
{
    this->_username = user;
}

// bool add_to_channel(Client *client, Channel channel)
// {
//     // if channel exists, and client has requirements for entering channel met
//     // add them to channel and
//     // channel_amount++;
// }


