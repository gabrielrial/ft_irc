#include "Bot.hpp"

Bot::Bot()
	: _ip("localhost"),
	  _port(6667),
	  _password("42"),
	  _nickname("nickname_bot"),
	  _username("username_bot"),
	  _realname("realname_bot"),
	  _hostname("hostname")
{
}

Bot::~Bot()
{
}

void Bot::init_connection()
{
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket == -1)
		throw std::runtime_error("Cannot create socket");

	_hint.sin_family = AF_INET;
	_hint.sin_port = htons(_port);

	// Intentar primero como dirección IP
	if (inet_pton(AF_INET, _ip.c_str(), &_hint.sin_addr) <= 0)
	{
		// Si falla, intentar resolver como hostname
		hostent *he = gethostbyname(_ip.c_str());
		if (he == NULL)
			throw std::runtime_error("Unknown host: " + _ip);

		std::memcpy(&_hint.sin_addr, he->h_addr, he->h_length);
	}

	if (connect(_socket, (sockaddr *)&_hint, sizeof(_hint)) < 0)
		throw std::runtime_error("Connection failure");
}

int Bot::run_bot()
{
	bot_registration();
	bot_readline();
	bot_closeconnection();
	return (0);
}

void Bot::bot_registration()
{
	send_message(_socket, "PASS " + _password);
	send_message(_socket, "NICK " + _nickname);
	send_message(_socket, "USER " + _username + " 0 * :" + _username);
}

void Bot::bot_readline()
{

	char buffer[512];

	while (true)
	{
		memset(buffer, 0, sizeof(buffer));
		int bytes_read = recv(_socket, buffer, sizeof(buffer) - 1, 0);
		if (bytes_read <= 0)
			break;
		buffer[bytes_read] = '\0';

		std::string line(buffer);
		if (line.find("!joke") != std::string::npos)
		{
			std::cout << "found" << std::endl;
			send_message(_socket, "PRIVMSG grial :Which is the scariest plant? Bamboo!!");
		}
	}
}

void Bot::bot_closeconnection()
{
	close(_socket);
}

void Bot::send_message(int socket_fd, const std::string &msg)
{
	std::string finalMsg = msg + "\r\n";
	send(socket_fd, finalMsg.c_str(), finalMsg.size(), 0);
}

const std::string &Bot::getIp() const {
    return _ip;
}

int Bot::get_socket() const {
    return _socket;
}

uint16_t Bot::getPort() const {
    return _port;
}

const std::string &Bot::getPassword() const {
    return _password;
}

const std::string &Bot::getNickname() const {
    return _nickname;
}

const std::string &Bot::getUsername() const {
    return _username;
}

const std::string &Bot::getRealname() const {
    return _realname;
}

const std::string &Bot::getHostname() const {
    return _hostname;
}