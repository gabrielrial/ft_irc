#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstdio>

const int PORT = 6667; 
const int BUFFER_SIZE = 512;

/**
 * Creates a TCP/IPv4 socket
 * 
 * @return Socket file descriptor or -1 on error
 */
int createSocket()
{
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1)
	{
		perror("socket failed");
	}
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
	if (listen(server_fd, 1) < 0)
	{
		perror("listen failed");
		return -1;
	}
	std::cout << "Listening on port " << PORT << "...\n";
	return 0;
}

/**
 * Accepts a new client connection
 * 
 * @param server_fd Server socket file descriptor
 * @param address Pointer to address structure
 * @param addrlen Pointer to address length
 * @return Client socket file descriptor or -1 on error
 */
int acceptClient(int server_fd, struct sockaddr_in* address, socklen_t* addrlen)
{
	//int client_fd = accept(server_fd, (struct sockaddr*)address, addrlen);
	int client_fd = accept(server_fd, reinterpret_cast<struct sockaddr*>(address), addrlen); //c6 jskehan
	if (client_fd < 0)
	{
		perror("accept failed");
		return -1;
	}
	std::cout << "Client connected!\n";
	return client_fd;
}

/**
 * Processes a single line received from the client
 * 
 * @param line The line to process
 */
void processLine(const std::string& line)
{
	std::cout << "RAW >>> " << line << std::endl;
	//IRC command processing
}

/**
 * Handles communication with a connected client
 * 
 * @param client_fd Client socket file descriptor
 */
void handleClientCommunication(int client_fd)
{
	char buffer[BUFFER_SIZE];
	std::string lineBuffer;

	while (true)
	{
		ssize_t bytes_read = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
		if (bytes_read <= 0)
		{
			std::cout << "Client disconnected.\n";
			break;
		}
		buffer[bytes_read] = '\0';
		lineBuffer.append(buffer);
		// Split by newline (\r\n in IRC)
		size_t pos;
		while ((pos = lineBuffer.find("\r\n")) != std::string::npos)
		{
			std::string line = lineBuffer.substr(0, pos);
			lineBuffer.erase(0, pos + 2);
			processLine(line);
		}
	}
}

/**
 * Cleans up resources
 * 
 * @param server_fd Server socket file descriptor
 * @param client_fd Client socket file descriptor
 */
void cleanup(int server_fd, int client_fd)
{
	if (client_fd >= 0)
		close(client_fd);
	if (server_fd >= 0)
		close(server_fd);
}

int main()
{
	int server_fd, client_fd;
	struct sockaddr_in address;
	socklen_t addrlen = sizeof(address);

	server_fd = createSocket();
	if (server_fd == -1)
		return 1;

	if (bindSocket(server_fd, &address) < 0)
	{
		cleanup(server_fd, -1);
		return 1;
	}

	if (startListening(server_fd) < 0)
	{
		cleanup(server_fd, -1);
		return 1;
	}

	client_fd = acceptClient(server_fd, &address, &addrlen);
	if (client_fd < 0)
	{
		cleanup(server_fd, -1);
		return 1;
	}

	handleClientCommunication(client_fd);

	cleanup(server_fd, client_fd);
	return 0;
}
