#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <cstdio>
#include <map>
#include "../parser/parser.hpp"

#define RES	"\033[0m"
#define RED	"\033[31m"
#define GRE	"\033[32m"
#define BLU	"\033[34m"
#define YEL	"\033[33m"

const int PORT = 6667;
const int BUFFER_SIZE = 512;
static std::map<int, std::string> clientNick;

/**
 * Creates a TCP/IPv4 socket with reuse address option
 * 
 * @return Socket file descriptor or -1 on error
 */
int createSocket()
{
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1)
	{
		perror("socket failed");
		return -1;
	}

	int opt = 1;
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	return server_fd;
}

/**
 * Configures and binds the socket to the specified port
 * 
 * @param server_fd Server socket file descriptor
 * @param address Pointer to address structure to be filled
 * @return 0 on success, -1 on error
 */
int bindSocket(int server_fd, struct sockaddr_in* address)
{
	memset(address, 0, sizeof(*address));
	address->sin_family = AF_INET;
	address->sin_addr.s_addr = INADDR_ANY;
	address->sin_port = htons(PORT);

	//if (bind(server_fd, (struct sockaddr*)address, sizeof(*address)) < 0)
	if (bind(server_fd, reinterpret_cast<struct sockaddr*>(address), sizeof(*address)) < 0) //c6 jskehan
	{
		perror("bind failed");
		return -1;
	}
	return 0;
}

/**
 * Puts the socket in listening mode
 * 
 * @param server_fd Server socket file descriptor
 * @return 0 on success, -1 on error
 */
int startListening(int server_fd)
{
	if (listen(server_fd, 5) < 0)
	{
		perror("listen failed");
		return -1;
	}
	std::cout << "Listening on port " << PORT << "...\n";
	return 0;
}

/**
 * Processes a line of text from a client
 * 
 * @param fd Client file descriptor
 * @param line Line of text to process
 */
void processLine(int fd, const std::string& line)
{
	std::cout << "RAW (fd=" << fd << ") >>> " << line << std::endl;
	// //IRC command processing
	// if (line.rfind("NICK ", 0) == 0)
	// 	clientNick[fd] = line.substr(5);
	// if (line.rfind("USER ", 0) == 0)
	// {
	// 	std::string nick = clientNick[fd].empty() ? "*" : clientNick[fd];
	// 	std::string welcome = ":localhost 001 " + nick + " :Welcome to mini_server\r\n";
	// 	send(fd, welcome.c_str(), welcome.size(), 0);
	// }

	RawTextLine parsed(line);
	std::cout << RED << "  Prefix: '" << parsed.getPrefix() << "'" << std::endl;
	std::cout << GRE << "  Command: '" << parsed.getCommand() << "'" << std::endl;
	std::cout << BLU << "  Params:";
	const std::vector<std::string>& params = parsed.getParams();
	for (std::vector<std::string>::const_iterator it = params.begin(); it != params.end(); ++it)
		std::cout << " '" << *it << "'";
	std::cout << std::endl;
	std::cout << YEL << "  Trailing: '" << parsed.getTrailing() << "'" << std::endl;
	std::cout << RES << std::endl;

	// Handle commands using parsed components
	std::string command = parsed.getCommand();
	if (command == "NICK" && !params.empty())
		clientNick[fd] = params[0];
	else if (command == "USER" && !params.empty())
	{
		std::string nick = clientNick[fd].empty() ? "*" : clientNick[fd];
		std::string welcome = ":localhost 001 " + nick + " :Welcome to mini_server\r\n";
		send(fd, welcome.c_str(), welcome.size(), 0);
	}
}

/**
 * Handles data received from a client
 * 
 * @param fd Client file descriptor
 * @param buffer Raw data buffer
 * @param bytes_read Number of bytes read
 * @param lineBuffer Reference to line buffer for this client
 */
void handleClientData(int fd, char* buffer, ssize_t bytes_read, std::string& lineBuffer)
{
	buffer[bytes_read] = '\0';
	lineBuffer.append(buffer);

	// Split by \r\n
	size_t pos;
	while ((pos = lineBuffer.find("\r\n")) != std::string::npos)
	{
		std::string line = lineBuffer.substr(0, pos);
		lineBuffer.erase(0, pos + 2);
		processLine(fd, line);
	}
}

/**
 * Accepts a new client connection
 * 
 * @param server_fd Server socket file descriptor
 * @param address Address structure
 * @param addrlen Address length
 * @param clients Vector of client file descriptors
 */
void acceptNewClient(int server_fd, struct sockaddr_in* address, socklen_t* addrlen, std::vector<int>& clients)
{
	//int new_fd = accept(server_fd, (struct sockaddr*)address, addrlen);
	int new_fd = accept(server_fd, reinterpret_cast<struct sockaddr*>(address), addrlen); //c6 jskehan
	if (new_fd >= 0)
	{
		clients.push_back(new_fd);
		std::cout << "New client connected (fd=" << new_fd << ")\n";
	}
}

/**
 * Prepares the fd_set for select() by adding all client sockets
 * 
 * @param server_fd Server socket file descriptor
 * @param clients Vector of client file descriptors
 * @param readfds fd_set to be filled
 * @return Highest file descriptor for select()
 */
int prepareFdSet(int server_fd, const std::vector<int>& clients, fd_set* readfds)
{
	FD_ZERO(readfds);
	FD_SET(server_fd, readfds);
	int max_fd = server_fd;
	
	for (std::vector<int>::const_iterator it = clients.begin(); it != clients.end(); ++it)
	{
		int fd = *it;
		FD_SET(fd, readfds);
		if (fd > max_fd)
			max_fd = fd;
	}
	
	return max_fd;
}

/**
 * Main server loop handling multiple clients with select()
 * 
 * @param server_fd Server socket file descriptor
 * @param address Server address structure
 * @param addrlen Address length
 */
void serverLoop(int server_fd, struct sockaddr_in* address, socklen_t* addrlen)
{
	std::vector<int> clients;
	char buffer[BUFFER_SIZE];
	std::string lineBuffer[FD_SETSIZE]; // one buffer per fd

	while (true)
	{
		fd_set readfds;
		int max_fd = prepareFdSet(server_fd, clients, &readfds);

		// Wait for activity
		int activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);
		if (activity < 0)
		{
			perror("select error");
			break;
		}
		// New connection
		if (FD_ISSET(server_fd, &readfds))
			acceptNewClient(server_fd, address, addrlen, clients);
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

/**
 * Cleans up resources
 * 
 * @param server_fd Server socket file descriptor
 */
void cleanup(int server_fd)
{
	close(server_fd);
}

int main()
{
	int server_fd;
	struct sockaddr_in address;
	socklen_t addrlen = sizeof(address);

	server_fd = createSocket();
	if (server_fd == -1)
		return 1;

	if (bindSocket(server_fd, &address) < 0)
	{
		cleanup(server_fd);
		return 1;
	}

	if (startListening(server_fd) < 0)
	{
		cleanup(server_fd);
		return 1;
	}

	serverLoop(server_fd, &address, &addrlen);

	cleanup(server_fd);
	return 0;
}
