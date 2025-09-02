#include "Server.hpp"

Server::Server()
{
	if (!set_bind())
		return;
	if (listen(_socket, SOMAXCONN) == -1)
		return;
}

Server::~Server()
{
}
/*
	It creats a binds the socket to an IP:Port,
	so that the server can listen on that address.
*/
bool Server::set_bind()
{
	this->_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socket == -1)
	{
		std::cerr << "Can not create a socket" << std::endl;
		return false;
	}

	memset(&_hint, 0, sizeof(_hint));
	this->_hint.sin_family = AF_INET;
	this->_hint.sin_port = htons(PORT);
	inet_pton(AF_INET, IP, &this->_hint.sin_addr);

	if (bind(_socket, (struct sockaddr *)&_hint, sizeof(_hint)) == -1)
	{
		std::cerr << "Bind failed" << std::endl;
		return false;
	}
	return true;
}

void Server::srv_run()
 {
	std::cout << "### srv_run(), only accepts one client" << std::endl;
	
	sockaddr_in	clientAddr;
	socklen_t clientSize = sizeof(clientAddr);

	char host[1025];
	char srv[32];

	int clientFd = accept(_socket, (struct sockaddr *)&clientAddr, &clientSize);
	if (clientFd == -1)
		std::cerr << "Problem with client" << std::endl;
	
	close (this->_socket);
	memset(host, 0, 1025);
	memset(srv, 0, 32);

	int result = getnameinfo((sockaddr *)&clientFd, sizeof(clientFd), host, 1025, srv, 32, 0);
	if (result)
	{
		std::cout << host << " connected on " << srv << std::endl
	}


}
