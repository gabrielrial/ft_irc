#include "Server.hpp"

Server::Server()
{
	try
	{
		init_socket();
		//	add_socket();
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
	char buffer[BUFFER_SIZE];
	std::string lineBuffer[FD_SETSIZE]; // one buffer per fd

	while (true)
	{
		fd_set readfds;
		int max_fd = prepareFdSet(clients, &readfds);

		// Wait for activity
		int activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);
		if (activity < 0)
		{
			std::cout << "srv_run() 1" << std::endl;
			break;
		}

		if (FD_ISSET(_socket, &readfds))
			acceptNewClient();
		// Existing clients
		for (std::vector<int>::iterator it = clients.begin(); it != clients.end();)
		{
			int fd = *it;
			if (FD_ISSET(fd, &readfds))
			{
				ssize_t bytes_read = recv(fd, buffer, BUFFER_SIZE - 1, 0);
				if (bytes_read <= 0)
				{
					std::cout << "Client (fd=" << fd << ") disconnected.\n";
					close(fd);
					it = clients.erase(it);
					continue;
				}
				handleClientData(fd, buffer, bytes_read, lineBuffer[fd]);
			}
			++it;
		}
	}
}

void Server::handleClientData(int fd, char *buffer, ssize_t bytes_read, std::string &lineBuffer)
{
	buffer[bytes_read] = '\0';
	lineBuffer.append(buffer);

	size_t pos;
	while ((pos = lineBuffer.find("\r\n")) != std::string::npos)
	{
		std::string line = lineBuffer.substr(0, pos);
		lineBuffer.erase(0, pos + 2);
		processLine(fd, line);
	}
}

void Server::processLine(int fd, const std::string &line)
{
	static std::map<int, std::string> clientNick; // nick por fd
	std::string nickname;
	std::string username;
	std::string hostname;
	std::string servername;
	std::string realname;

	std::cout << "RAW (fd=" << fd << ") >>> " << line << std::endl;

	std::cout << "what happens if i change my nickname during the execution?"<< std::endl;
	if (line.rfind("NICK ", 0) == 0)
	{
		size_t end = line.find_first_of(" \r\n", 5);
		nickname = line.substr(5, end - 5);
		clientNick[fd] = nickname; // guardar nick por fd
		std::cout << nickname << std::endl;
	}
	if (line.rfind("USER ", 0) == 0)
	{
		size_t start = 5;
		size_t end = line.find_first_of(" \r\n", start);
		if (end == std::string::npos)
			return;
		username = line.substr(start, end - start);

		start = end + 1;
		end = line.find_first_of(" \r\n", start);
		if (end == std::string::npos)
			return;
		hostname = line.substr(start, end - start);

		start = end + 1;
		end = line.find_first_of(" \r\n", start);
		if (end == std::string::npos)
			return;
		servername = line.substr(start, end - start);

		// Todo lo que sigue después de ':' es realname
		size_t colon = line.find(':', start);
		if (colon != std::string::npos)
			realname = line.substr(colon + 1);

		// Usar el nick guardado si existe, sino '*'
		nickname = clientNick.count(fd) ? clientNick[fd] : "*";

		// Enviar welcome 001
		std::string welcome =
			":localhost 001 " + nickname + " :Welcome to mini_server " + nickname + "\r\n";

		send(fd, welcome.c_str(), welcome.size(), 0);

		// Debug
		std::cout << "Username: " << username << std::endl;
		std::cout << "Hostname: " << hostname << std::endl;
		std::cout << "Servername: " << servername << std::endl;
		std::cout << "Realname: " << realname << std::endl;
		std::cout << "===============================================" << std::endl;
	}
}

void Server::acceptNewClient()
{
	sockaddr_in clientAddr;
	socklen_t clientSize = sizeof(clientAddr);

	int new_fd = accept(_socket, (struct sockaddr *)&clientAddr, &clientSize);
	if (new_fd >= 0)
	{
		clients.push_back(new_fd);
		std::cout << "New client connected (fd=" << new_fd << ")\n";
	}
}

// void Server::add_socket()
//{
//	pollfd srv_fd;
//
//	srv_fd.fd = _socket;
//	srv_fd.events = POLLIN;
//	srv_fd.revents = 0;
//
//	_fds.push_back(srv_fd);
// }

int Server::prepareFdSet(const std::vector<int> &clients, fd_set *readfds)
{
	FD_ZERO(readfds);
	FD_SET(_socket, readfds);
	int max_fd = _socket;

	for (std::vector<int>::const_iterator it = clients.begin(); it != clients.end(); ++it)
	{
		int fd = *it;
		FD_SET(fd, readfds);
		if (fd > max_fd)
			max_fd = fd;
	}

	return max_fd;
}
