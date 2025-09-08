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
	It creats and binds the socket to an IP:Port,
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
	this->_hint.sin6_family = AF_INET6;
	this->_hint.sin6_port = htons(PORT);
	inet_pton(AF_INET, IP, &this->_hint.sin6_addr);
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
	int 		clientSocket;
	sockaddr_in clientAddr;
	socklen_t clientSize = sizeof(clientAddr);
	clientSize = sizeof(clientAddr);
	clientSocket = accept(this->_socket, (struct sockaddr *)&clientAddr, &clientSize);
	std::cout << "<ClienteName> connected" << std::endl;
	char buffer[BUFFER_SIZE];
	char clientIP[INET_ADDRSTRLEN];
	if (getpeername(clientSocket, (struct sockaddr*)&clientAddr, &clientSize) == 0)
	{
    	inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, sizeof(clientIP));
    	int clientPort = ntohs(clientAddr.sin_port);
  	  std::cout << "Client connected from " << clientIP << ":" << clientPort << std::endl;
	}
	while (1)
	{
        memset(buffer, '\0', BUFFER_SIZE);
		int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
		if (bytesReceived > 0)
		{
            buffer[bytesReceived] = '\0';
			std::cout << "<ClienteName> says: " << buffer << std::endl;
			
			std::string reply = "Hi, <ClienteName>!\n";
			send(clientSocket, reply.c_str(), reply.size(), 0);
		}	
	}
    close(this->_socket);
	close(clientSocket);
}