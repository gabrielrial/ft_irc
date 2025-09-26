#include "Bot.hpp"

Bot::Bot() : _nickname("nickname_bot"), _username("username_bot"), _realname("realname_bot"), _hostname("hostname"), _password("42"), _ip("localhost")
{
}

void Bot::init_connection()
{
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket == -1)
		throw std::runtime_error("Cannot create socket");

	_hint.sin_family = AF_INET;
	_hint.sin_port = htons(_port);
	inet_pton(AF_INET, _ip, &_hint.sin_addr);

	if (connect(_socket, (sockaddr *)&_hint, sizeof(_hint)) < 0)
		throw std::runtime_error("Connection failure");
}

int Bot::run_bot()
{
	bot_registration();

	char buffer[512];
	while (true)
	{
		memset(buffer, 0, sizeof(buffer));
		int bytes_read = recv(_socket, buffer, sizeof(buffer) - 1, 0);
		if (bytes_read <= 0)
			break;
		buffer[bytes_read] = '\0';

		std::string line(buffer);
		handle_client_data(fd, buffer, bytes_read, lineBuffer[fd]);
		if (line.find("!jock"))
		{
			std::cout << "found" << std::endl;
			sendMessage(socket_fd, "PRIVMSG grial :Which is the scariest plant? Bamboo!!");
		}
	}
}

void Bot::bot_registration()
{
	send_message(_socket, "PASS " + _password);
	send_message(_socket, "NICK " + _nickname);
	send_message(_socket, "USER " + _username + " 0 * :" + _username);
}

void Bot::send_message(int socket_fd, const std::string &msg)
{
	std::string finalMsg = msg + "\r\n";
	send(socket_fd, finalMsg.c_str(), finalMsg.size(), 0);
}
