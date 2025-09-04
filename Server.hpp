#ifndef SERVER_HPP
#define SERVER_HPP

#include "lib_irc.hpp"
#include <netinet/in.h>
#include <string>
#include <vector>
#include <poll.h>
#include "Client.hpp"

class Server
{
private:
	int _socket;
	sockaddr_in _hint;
	//pollfd _serverfd;
	std::vector<Client> _clients;
	std::vector<pollfd> _fds;

	void init_socket();
	void create_socket();
	void bind_socket();
	void start_listening();
	void add_socket();

	void fillPollFd();

	void acceptClient();


public:
	Server();
	~Server();

	void srv_run();
};

#endif