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

	int 		clientSocket;
	sockaddr_in clientAddr;
	socklen_t clientSize = sizeof(clientAddr);

	clientSize = sizeof(clientAddr);
	clientSocket = accept(this->_socket, (struct sockaddr *)&clientAddr, &clientSize);
	close(this->_socket); // We can not close it in order to recive more connections.


	std::cout << "cliente connected" << std::endl;
	char buffer[BUFFER_SIZE];
	while (1)
	{
		memset(buffer, '\0', BUFFER_SIZE);
		int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
		if (bytesReceived > 0)
		{
			buffer[bytesReceived] = '\0';
			std::cout << "Cliente dijo: " << buffer << std::endl;
			
			// Responder al cliente
			std::string reply = "Hola, cliente!\n";
			send(clientSocket, reply.c_str(), reply.size(), 0);
		}	
	}
	// Cerrar la conexión con el cliente
	close(clientSocket);
}

