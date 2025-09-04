#ifndef SERVER_HPP
#define SERVER_HPP

#include "lib_irc.hpp"
#include <netinet/in.h>
#include <string>

class Server
{
private:
	int _socket;
	sockaddr_in _hint;

	void init_socket();
	void create_socket();
	void bind_socket();
	void start_listening();

public:
	Server();
	~Server();

	void srv_run();
};

#endif