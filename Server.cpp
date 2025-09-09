#include "Server.hpp"

Server::Server()
{
	try
	{
		init_socket();
		add_socket();
		std::cout << "Server listening on " << IP << ":" << PORT << std::endl;
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
	while (1)
	{
		int res = poll(_fds.data(), _fds.size(), -1);
		if (res < 0)
		{
			std::cerr << "poll failed" << std::endl;
			break;
		}
		if (_fds[0].revents & POLLIN)
		{
			sockaddr_in clientAddr;
			socklen_t clientSize = sizeof(clientAddr);
			int newClient = accept(_socket, (struct sockaddr *)&clientAddr, &clientSize);
			if (newClient != -1)
			{
				_clients.push_back(Client(newClient));
				pollfd newfd;
				newfd.fd = newClient;
				newfd.events = POLLIN;
				_fds.push_back(newfd);
				std::cout << "<ClientName> has connected" << newClient << std::endl;
			}
		}
		for (size_t i = 1; i < _fds.size(); i++)
		{
			if (_fds[i].fd != -1 && (_fds[i].revents & POLLIN))
			{
				char buffer[1024];
				int bytes = recv(_fds[i].fd, buffer, sizeof(buffer) - 1, 0);
				if (bytes <= 0)
				{
					// cliente desconectó
					std::cout << "Client disconnected: " << _fds[i].fd << std::endl;
					close(_fds[i].fd);
					_fds[i].fd = -1;
				}
				else
				{
					buffer[bytes] = '\0';
					std::cout << "Client says: " << buffer << std::endl;

					// opcional: enviar respuesta
					std::string reply = "Hi client!\n";
					send(_fds[i].fd, reply.c_str(), reply.size(), 0);
				}
			}
		}
	}
	close(this->_socket);
}

void Server::acceptClient()
{
	sockaddr_in clientAddr;
	socklen_t clientSize = sizeof(clientAddr);
	int clientSocket = accept(_socket, (struct sockaddr *)&clientAddr, &clientSize);

	if (clientSocket != -1)
	{
		Client newClient(clientSocket); // creats a client with its socket
		_clients.push_back(newClient);	// add it to the container
		std::cout << "<ClientName> has connected" << clientSocket << std::endl;
	}
}

void Server::add_socket()
{
	pollfd srv_fd;

	srv_fd.fd = _socket;
	srv_fd.events = POLLIN;
	srv_fd.revents = 0;

	_fds.push_back(srv_fd);
}

void Server::fillPollFd()
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		pollfd pfd;
		pfd.fd = _clients[i].getSocket();
		pfd.events = POLLIN;
		pfd.revents = 0;
		_fds.push_back(pfd);
	}
}
