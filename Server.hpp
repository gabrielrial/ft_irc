#ifndef SERVER_HPP
#define SERVER_HPP

#include "lib_irc.hpp"
#include <netinet/in.h>

class Server
{
private:
	int _socket;
	sockaddr_in _hint;

	bool set_bind();
	
	public:
	Server();
	~Server();

	void srv_run();
};

#endif