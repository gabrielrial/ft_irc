#ifndef SERVER_HPP
#define SERVER_HPP

#include "lib_irc.hpp"
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <map>
#include <poll.h>

class Server
{
private:
	int _socket;
	sockaddr_in _hint;
	std::vector<int> clients;

	//bool set_bind(); //one conn
	void init_socket();
	void create_socket();
	void bind_socket();
	void start_listening();
	//void add_socket();

	int prepareFdSet(const std::vector<int>& clients, fd_set *readfds);

	void acceptNewClient();

	void processLine(int fd, const std::string& line);
	void handleClientData(int fd, char* buffer, ssize_t bytes_read, std::string& lineBuffer);



public:
	Server();
	~Server();

	void srv_run();
};

#endif