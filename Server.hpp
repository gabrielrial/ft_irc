#ifndef SERVER_HPP
#define SERVER_HPP

#include "lib_irc.hpp"
#include <netinet/in.h>
#include <string>
#include <vector>
#include <map>
#include <poll.h>
#include "Client.hpp"
#include "Command.hpp"

class Server
{
private:
	int _socket;
	sockaddr_in6 _hint;
	std::vector<Client> _clients;
	std::vector<pollfd> _fds;
	std::map<std::string, std::string> _nicknames;  // nickname -> real name
	std::map<std::string, std::vector<Client*> > _channels;  // channel -> clients

	//bool set_bind(); //one conn
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