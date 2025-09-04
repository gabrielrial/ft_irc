#include "Server.hpp"

Server::Server()
{
	try
	{
		init_socket();
	}
	catch (const std::runtime_error &e)
	{
		std::cerr << "Server initialization failed: " << e.what() << std::endl;
		throw;
	}
}

Server::~Server()
{
}

/*
	Creates a TCP/IPv4 socket, binds it to the server's IP and port,
	sets it to listen for incoming connections, and allows address reuse.
*/
void Server::init_socket()
{
	try
	{
		create_socket();
		bind_socket();
		start_listening();
	}
	catch (const std::runtime_error &e)
	{
		std::cerr << "Server initialization failed: " << e.what() << std::endl;
		if (_socket != -1)
			close(_socket);
		throw;
	}

	std::cout << "Server listening on " << IP << ":" << PORT << std::endl;
}

void Server::create_socket()
{
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket == -1)
		throw std::runtime_error("Cannot create socket");

	int yes = 1;
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
		throw std::runtime_error("setsockopt failed");
}

void Server::bind_socket()
{
	memset(&_hint, 0, sizeof(_hint));
	_hint.sin_family = AF_INET;				 // IPv4
	_hint.sin_port = htons(PORT);			 // convert port to network byte order
	inet_pton(AF_INET, IP, &_hint.sin_addr); // convert IP string to binary form

	if (bind(_socket, (struct sockaddr *)&_hint, sizeof(_hint)) == -1)
		throw std::runtime_error("Bind failed");
}

void Server::start_listening()
{
	if (listen(_socket, SOMAXCONN) == -1)
		throw std::runtime_error("Listen failed");
}

void Server::srv_run()
{
	std::cout << "### srv_run(), only accepts one client" << std::endl;

	int clientSocket;
	sockaddr_in clientAddr;
	socklen_t clientSize = sizeof(clientAddr);

	clientSize = sizeof(clientAddr);
	clientSocket = accept(this->_socket, (struct sockaddr *)&clientAddr, &clientSize);

	std::cout << "<ClienteName> connected" << std::endl;
	char buffer[BUFFER_SIZE];
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
