#include "Client.hpp"

Client::Client(int fd) : _client_fd(fd), _channel_amount(0),
        _nickname("*"), _username(""), _realname(""), channels(0) {}

Client::~Client()
{
    if (_client_fd > 0)
        close(_client_fd);
    return ;
}
//              getters
int Client::getFd() const
{
    return _client_fd;
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

std::string Client::getRealname() const
{
    return _realname;
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


