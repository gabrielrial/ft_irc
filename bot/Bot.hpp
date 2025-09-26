#pragma once

#include <netinet/in.h> // sockaddr_in
#include <sys/socket.h>
#include <string>

#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "../lib_irc.hpp"

class Bot
{
private:
	char *_ip;
	int 	_socket;
	uint16_t _port;
	sockaddr_in _hint;
	std::string _password;
	hostent *_host;

	const std::string _nickname;
	const std::string _username;
	const std::string _realname;
	const std::string _hostname;

public:
	Bot();
	~Bot();

	void	init_connection();
	int		run_bot();
	void	bot_registration();
	void	send_message(int socket_fd, const std::string &msg);
};

