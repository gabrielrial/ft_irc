#include "Client.hpp"

Client::Client(int fd, sockaddr_in &addr) : _pong(true), _client_fd(fd), _nickname("*"), _username("*"),
										 _realname(""), _has_pass(false), _has_nick(false),
											_has_user(false), _visible(true), _operator(false), channels(0)
{
	char ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &addr.sin_addr, ip, sizeof(ip));
	_ip = ip;
}

Client::~Client()
{
}
//              getters
int Client::get_FD() const
{
	return _client_fd;
}

std::string Client::get_IP() const
{
	return _ip;
}

bool Client::has_pass() const
{
	return _has_pass;
}

bool Client::has_nick() const
{
	return _has_nick;
}

bool Client::has_user() const
{
	return _has_user;
}

void Client::pass_accepted(bool accepted)
{
	_has_pass = accepted;
}

std::string Client::get_nickname() const
{
	return _nickname;
}

std::string Client::get_username() const
{
	return _username;
}

std::string Client::get_realname() const
{
	return _realname;
}

std::string Client::get_hostname() const
{
	return _hostname;
}

std::string Client::get_servername() const
{
	return _servername;
}

void Client::set_visible()
{
	if (!_visible)
		_visible = true;
	else
		_visible = false;
}

bool Client::get_visible() const
{
	return _visible;
}

bool Client::is_operator()
{
	return _operator;
}

std::string Client::get_prefix() const
{
    const std::string& n = _nickname.empty() ? "*" : _nickname;
	const std::string& u = _username.empty() ? "*" : _username;
	const std::string& h = _hostname.empty() ? _ip : _hostname;
	return std::string(":") + n + "!" + u + "@" + h;
}


//              setters
void Client::set_nickname(const std::string &nick)
{
	this->_nickname = nick;
	this->_has_nick = true;
}

void Client::set_username(const std::string &user)
{
	this->_username = user;
	this->_has_user = true;
}
void Client::set_realname(const std::string &real)
{
	this->_realname = real;
}

void Client::set_hostname(const std::string &host)
{
	this->_hostname = host;
}

void Client::set_servername(const std::string &serv)
{
	this->_servername = serv;
}

bool Client::is_registered()
{
	return _has_pass && _has_nick && _has_user;
}

void	Client::set_operator(bool set)
{
	this->_operator = set;
}

// bool add_to_channel(Client *client, Channel channel)
// {
//     // if channel exists, and client has requirements for entering channel met
//     // add them to channel and
// }

bool Client::operator==(const Client &other) const
{
	return this->get_FD() == other.get_FD();
}

//time_t	Client::get_lastpong()
//{
//	return _last_pong;
//}
//
//void	Client::set_lastpong()
//{
//	_last_pong = std::time(NULL);
//}

bool Client::get_pong() const
{
	return _pong;
}

void Client::set_pong()
{
	if (!_pong)
		_pong = true;
	else
		_pong = false;
}
