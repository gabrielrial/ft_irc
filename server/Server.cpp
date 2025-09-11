#include "Server.hpp"

#define RES	"\033[0m"
#define RED	"\033[31m"
#define GRE	"\033[32m"
#define BLU	"\033[34m"
#define YEL	"\033[33m"

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

/**
 * @brief Creates and configures a TCP socket for IPv6 communication
 * 
 * This method creates a new IPv6 TCP socket and sets the SO_REUSEADDR option
 * to allow reuse of local addresses. The socket is stored in the _socket member variable.
 * 
 * @throws std::runtime_error If socket creation fails
 * @throws std::runtime_error If setting socket options fails
 */
void Server::create_socket()
{
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket == -1)
		throw std::runtime_error("Cannot create socket");

	int yes = 1;
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
		throw std::runtime_error("setsockopt failed");
}

/**
 * @brief Binds the server socket to a specific IPv6 address and port.
 * 
 * This function configures the socket address structure with IPv6 family,
 * converts the port to network byte order, and converts the IP address
 * from string format to binary form. Then binds the socket to the
 * specified address and port.
 * 
 * @throws std::runtime_error if the bind operation fails
 * 
 * @note Uses predefined PORT and IP constants/macros
 * @note The _hint structure is zeroed out before configuration
 */
void Server::bind_socket()
{
	memset(&_hint, 0, sizeof(_hint));
	_hint.sin_family = AF_INET;				 // IPv4
	_hint.sin_port = htons(PORT);			 // convert port to network byte order
	inet_pton(AF_INET, IP, &_hint.sin_addr); // convert IP string to binary form

	if (bind(_socket, (struct sockaddr *)&_hint, sizeof(_hint)) == -1)
		throw std::runtime_error("Bind failed");
}

/**
 * @brief Starts listening for incoming connections on the server socket
 * 
 * This method puts the server socket into listening mode, allowing it to accept
 * incoming client connections. The socket must be bound to an address before
 * calling this function.
 * 
 * @throws std::runtime_error if the listen() system call fails
 * 
 * @note Uses SOMAXCONN as the maximum number of pending connections in the queue
 */
void Server::start_listening()
{
	if (listen(_socket, SOMAXCONN) == -1)
		throw std::runtime_error("Listen failed");
}

/**
 * @brief Main server loop that handles client connections and communication
 * 
 * This function runs the server's main event loop using poll() to monitor file descriptors.
 * It continuously:
 * - Waits for events on the server socket and client sockets
 * - Accepts new client connections when they arrive on the server socket
 * - Reads incoming messages from connected clients
 * - Sends acknowledgment responses to clients
 * - Handles client disconnections by cleaning up resources
 * 
 * The function maintains a vector of pollfd structures (_fds) where index 0 is the server
 * socket and subsequent indices are client sockets. It also maintains a parallel vector
 * of Client objects (_clients) to track client state.
 * 
 * @note This function runs indefinitely until a poll() error occurs
 * @note The server socket is closed when the function exits
 * @warning This function blocks indefinitely waiting for socket events
 */
void Server::srv_run()
{
	char buffer[BUFFER_SIZE];
	std::string lineBuffer[FD_SETSIZE]; // one buffer per fd

	while (true)
	{
		fd_set readfds;
		int max_fd = prepareFdSet(clients, &readfds);

		int activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);
		if (activity < 0)
		{
			std::cout << "srv_run() 1" << std::endl;
			break;
		}

		// New Clients
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

	RawTextLine parsed(line);
	std::cout << RED << "  Prefix: '" << parsed.getPrefix() << "'" << std::endl;
	std::cout << GRE << "  Command: '" << parsed.getCommand() << "'" << std::endl;
	std::cout << BLU << "  Params:";
	const std::vector<std::string>& params = parsed.getParams();
	for (std::vector<std::string>::const_iterator it = params.begin(); it != params.end(); ++it)
		std::cout << " '" << *it << "'";
	std::cout << std::endl;
	std::cout << YEL << "  Trailing: '" << parsed.getTrailing() << "'" << std::endl;
	std::cout << BLU << "  Separated Params:";
	const std::vector<std::string>& sep_params = parsed.getSepParams();
	for (std::vector<std::string>::const_iterator it = sep_params.begin(); it != sep_params.end(); ++it)
		std::cout << " '" << *it << "'";
	std::cout << std::endl;
	std::cout << RES << std::endl;
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

bool Server::check_client(RawTextLine &line)
{
	for (size_t i = 0; i < clients.size(); i++)
	{
		for (size_t a = 0; a < line.getSepParams().size(); a++)
			if (line.getSepParams()[a] == clients[i].getNickname())
				return true;
	}
	return false;
}

bool Server::check_channel(RawTextLine &line)
{
	for (size_t i = 0; i < channels.size(); i++)
	{
		for (size_t a = 0; a < line.getSepParams().size(); a++)
			if (line.getSepParams()[a] == channels[i].getName())
				return true;
	}
	return false;
}
