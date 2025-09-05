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
	_socket = socket(AF_INET6, SOCK_STREAM, 0);
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
	_hint.sin6_family = AF_INET6;				 // IPv4
	_hint.sin6_port = htons(PORT);			 // convert port to network byte order
	inet_pton(AF_INET6, IP, &_hint.sin6_addr); // convert IP string to binary form

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
	while (1)
	{
		int res = poll(_fds.data(), _fds.size(), -1);
		//int res = poll(_fds.data(), _fds.size(), 1000);
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
				//a++;
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

/**
 * @brief Adds the server socket to the poll file descriptor list for monitoring incoming connections.
 * 
 * This function creates a pollfd structure for the server socket and adds it to the internal
 * file descriptor vector (_fds). The socket is configured to monitor for POLLIN events,
 * which indicates when new client connections are available to accept.
 * 
 * @note This function should typically be called during server initialization after the
 *       server socket has been created and bound to a port.
 */
void Server::add_socket()
{
	pollfd srv_fd;

	srv_fd.fd = _socket;
	srv_fd.events = POLLIN;
	srv_fd.revents = 0;

	_fds.push_back(srv_fd); //inline member function of std::vector class template for pollfd elements. Appends a copy of the given value_type (pollfd obj) to the end, increasing size by one.
}

/**
 * @brief Fills the polling file descriptor vector with client socket information.
 * 
 * This function iterates through all connected clients and creates pollfd structures
 * for each client socket. Each pollfd is configured to monitor for incoming data
 * (POLLIN events) and is added to the _fds vector for use with poll() system call.
 * The revents field is initialized to 0 for each entry.
 * 
 * @note This function should be called before polling to ensure all client sockets
 *       are monitored for incoming data.
 * @note The _fds vector should be cleared before calling this function if it
 *       contains previous polling data.
 */
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
