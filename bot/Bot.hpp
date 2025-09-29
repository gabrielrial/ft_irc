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
	const std::string _ip;
	int _socket;
	uint16_t _port;
	sockaddr_in _hint;
	const std::string _password;

	const std::string _nickname;
	const std::string _username;
	const std::string _realname;
	const std::string _hostname;

public:
	Bot();
	~Bot();

	void init_connection();
	void bot_registration();
	int run_bot();
	void bot_readline();
	void bot_closeconnection();
	void send_message(int socket_fd, const std::string &msg);

	const std::string &getIp() const;
	int get_socket() const;
	uint16_t getPort() const;
	const std::string &getPassword() const;
	const std::string &getNickname() const;
	const std::string &getUsername() const;
	const std::string &getRealname() const;
	const std::string &getHostname() const;
};
