#include "Client.hpp"

Client::Client(int fd, sockaddr_in &addr) : _client_fd(fd), _channel_amount(0),
        _nickname("*"), _username("*"), _realname(""), _has_pass(false), _has_nick(false),
        _has_user(false), channels(0)
{
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &addr.sin_addr, ip, sizeof(ip));
    _ip = ip;
}

Client::~Client()
{
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

bool    Client::hasPass() const
{
    return _has_pass;
}

void    Client::passAccepted(bool accepted)
{
    _has_pass = accepted;
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
void Client::setNickname(const std::string &nick)
{
	this->_nickname = nick;
}

void Client::setUsername(const std::string &user)
{
	this->_username = user;
}
void Client::setRealname(const std::string &real)
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
    return _has_pass && _has_nick && _has_user;
}


// bool add_to_channel(Client *client, Channel channel)
// {
//     // if channel exists, and client has requirements for entering channel met
//     // add them to channel and
//     // channel_amount++;
// }

bool Client::operator==(const Client& other) const
{
	return this->getFd() == other.getFd();
}
