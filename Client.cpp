#include "Client.hpp"

Client::Client(int fd, sockaddr_in &addr) : _client_fd(fd), _channel_amount(0),
        _nickname("*"), _username("*"), _realname(""), _has_nick(false),
        _has_user(false), channels(0)
{
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &addr.sin_addr, ip, sizeof(ip));
    _ip = ip;
}

Client::~Client()
{
    if (_client_fd >= 0)
    {
        std::cout << "Closing fd=" << _client_fd << std::endl;
        close(_client_fd);
       _client_fd = -1;
    }
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

std::string Client::getHostname() const
{
    return _hostname;
}

std::string Client::getServername() const
{
    return _servername;
}

int Client::get_channel_amt()
{
    return this->_channel_amount;
}

//              setters
void    Client::setNickname(const std::string &nick)
{
    this->_nickname = nick;
    _has_nick = true;
}

void    Client::setUsername(const std::string &user)
{
    this->_username = user;
}

void    Client::setRealname(const std::string &real)
{
    this->_realname = real;
}

void    Client::setHostname(const std::string &host)
{
    this->_hostname = host;
}

void    Client::setServername(const std::string &serv)
{
    this->_servername = serv;
}

bool    Client::isRegistered()
{
    if ((_has_nick == true) && (_has_user == true))
        return true;
    return false;
}


