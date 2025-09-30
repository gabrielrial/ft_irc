#include "Server.hpp"

#define RES "\033[0m"
#define RED "\033[31m"
#define GRE "\033[32m"
#define BLU "\033[34m"
#define YEL "\033[33m"

Server::Server(uint16_t port, std::string password)
	: _port(port), _password(password), _socket(-1), client_amt(0), _dcc_manager(new DCCManager())
{
	set_servername();
	try
	{
		init_socket();
		//	add_socket();
		std::cout << "Server listening on " << IP << ":" << port << std::endl;
	}
	catch (const std::runtime_error &e)
	{
		std::cerr << "Server initialization failed: " << e.what() << std::endl;
		throw;
	}
}

Server::~Server()
{
	delete _dcc_manager;
	_dcc_manager = NULL;
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

	// make server socket non-blocking
	int flags = fcntl(_socket, F_GETFL, 0);
	if (flags == -1)
		flags = 0;
	if (fcntl(_socket, F_SETFL, flags | O_NONBLOCK) == -1)
		throw std::runtime_error("fcntl failed");
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
	_hint.sin_port = htons(_port);			 // convert port to network byte order
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

	while (g_running)
	{
		remove_closed_clients(lineBuffer);
		fd_set readfds;
		int max_fd = prepare_fd_set(&readfds);

		struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = 100000; // 100ms timeout (prevents CPU spin)

		int activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);
		if (activity < 0)
		{
			if (errno == EINTR)
				continue; // just retry if interrupted
			perror("select");
			break;
		}
		// New Clients
		if (FD_ISSET(_socket, &readfds))
			register_client();

		// Existing clients
		for (size_t i = 0; i < clients.size();)
		{
			int fd = clients[i].get_FD();
			if (FD_ISSET(fd, &readfds))
			{
				ssize_t bytes_read = recv(fd, buffer, BUFFER_SIZE - 1, 0);
				/* if (bytes_read == 0)
				{
					// std::cout << "Client (fd=" << fd << ") disconnected.\n";
					//handle_disconnection(fd, bytes_read == 0 ? "Connection closed" : strerror(errno));
					//close(fd);
					//clients.erase(clients.begin() + i);
					continue;
				}
				if (bytes_read < 0)
				{
					if (errno == EAGAIN || errno == EWOULDBLOCK)
					{
						++i;
						continue;
					}
					//std::cout << "Client (fd=" << fd << ") disconnected.\n";
					//handle_disconnection(fd, bytes_read == 0 ? "Connection closed" : strerror(errno));
					//close(fd);
					//clients.erase(clients.begin() + i);
					continue;
				} */
				if (bytes_read <= 0)  // Both EOF and errors (except EAGAIN/EWOULDBLOCK)
				{
					if (bytes_read < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
					{
						++i;
						continue;
					}
					handle_disconnection(fd, bytes_read == 0 ? "Connection closed" : strerror(errno));
					continue;
				}
				buffer[bytes_read] = '\0';
				handle_client_data(fd, buffer, bytes_read, lineBuffer[fd]);
			}
			++i;
		}

		// Check and process DCC transfers
		_dcc_manager->check_transfers();

		remove_closed_clients(lineBuffer);
	}
}

void Server::handle_client_data(int fd, char *buffer, ssize_t bytes_read, std::string &lineBuffer)
{
	buffer[bytes_read] = '\0';
	lineBuffer.append(buffer);

	size_t pos;
	while ((pos = lineBuffer.find("\r\n")) != std::string::npos)
	{
		std::string line = lineBuffer.substr(0, pos);
		lineBuffer.erase(0, pos + 2);
		if (line.find("DCC ") != std::string::npos) // Check if this is a DCC message
		{
			Client *client = find_client(fd);
			if (client)
			{
				try
				{
					process_line(fd, line);
				}
				catch (const std::exception &e)
				{
					std::string error = "ERROR :DCC error: " + std::string(e.what()) + "\r\n";
					send(fd, error.c_str(), error.length(), 0);
				}
			}
		}
		else
			process_line(fd, line);
	}
}

void Server::process_line(int fd, const std::string &line) // change to parse through potential commands and use cmd_nick etc
{
	Client *client = find_client(fd);

	std::cout << "RAW (fd=" << fd << ") >>> " << line << std::endl;
	RawTextLine parsed(line);
	// std::cout << line << std::endl;
	run_cmds(*this, parsed, *client);
}

void Server::register_client()
{
	sockaddr_in clientAddr;
	socklen_t clientSize = sizeof(clientAddr);

	int new_fd = accept(_socket, (struct sockaddr *)&clientAddr, &clientSize);
	if (new_fd < 0)
	{
		perror("accept");
		return;
	}
	int flags = fcntl(new_fd, F_GETFL, 0);
	if (flags == -1)
		flags = 0;
	if (fcntl(new_fd, F_SETFL, flags | O_NONBLOCK) == -1)
	{
		close(new_fd);
		return;
	}
	Client c(new_fd, clientAddr);
	char host[NI_MAXHOST];
	char serv[NI_MAXSERV];
	if (getnameinfo((struct sockaddr *)&clientAddr, sizeof(clientAddr),
					host, sizeof(host), serv, sizeof(serv), 0) == 0)
		c.set_hostname(host);
	else
		c.set_hostname("unknown");
	char serverName[256];
	if (gethostname(serverName, sizeof(serverName)) != 0)
		strcpy(serverName, "localhost");
	c.set_servername(serverName);
	clients.push_back(c);
	std::cout << "New client connected (fd=" << new_fd << ")\n";
}

Client *Server::find_client(int fd)
{
	for (size_t i = 0; i < clients.size(); i++)
	{
		if (fd == clients[i].get_FD())
			return &clients[i];
	}
	return NULL;
}

void Server::welcome(Client client)
{
	std::string welcome = ":localhost 001 " + client.get_nickname() + " :Welcome to mini_server " + client.get_nickname() + "\r\n";
	send(client.get_FD(), welcome.c_str(), welcome.size(), 0);
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

int Server::prepare_fd_set(fd_set *readfds)
{
	FD_ZERO(readfds);
	FD_SET(_socket, readfds);
	int max_fd = _socket;

	for (size_t i = 0; i < clients.size(); i++)
	{
		int fd = clients[i].get_FD();
		FD_SET(fd, readfds);
		if (fd > max_fd)
			max_fd = fd;
	}
	return max_fd;
}

void Server::set_client_amt()
{
	this->client_amt++;
}

void Server::remove_closed_clients(std::string lineBuffer[])
{
	for (size_t i = 0; i < _fdsToClose.size(); ++i)
	{
		int deadFd = _fdsToClose[i];
		lineBuffer[deadFd].clear();
		close(deadFd);
		for (size_t j = 0; j < clients.size();)
		{
			if (clients[j].get_FD() == deadFd)
				clients.erase(clients.begin() + j);
			else
				++j;
		}
	}
	_fdsToClose.clear();
}

void Server::check_client(RawTextLine &line, std::vector<Client *> &client_list)
{
	for (size_t i = 0; i < clients.size(); i++)
	{
		for (size_t a = 0; a < line.get_sep_params().size(); a++)
		{
			if (line.get_sep_params()[a] == clients[i].get_nickname())
				client_list.push_back(&clients[i]); // guardamos puntero al cliente
		}
	}
}

bool Server::check_channel(RawTextLine &line)
{
	const std::vector<std::string> &channels_to_check = line.get_sep_params();
	for (size_t i = 0; i < channels_to_check.size(); i++)
	{
		if (get_channel(channels_to_check[i]) != NULL)
			return true;
	}
	return false;
}

// like check_channel but returns the channel
Channel *Server::get_channel(const std::string &name)
{
	std::string chanel_name = name;
	if (name[0] != '#')
		chanel_name = "#" + name;

	for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		if (it->get_name() == chanel_name)
			return &(*it);
	}
	return NULL;
}

Client *Server::get_client(const std::string &name)
{
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->get_nickname() == name)
			return &(*it);
	}
	return NULL;
}

void Server::add_channel(const std::string &name)
{
	for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		if (it->get_name() == name)
			return;
	}
	Channel newChannel(name);
	channels.push_back(newChannel);
}

int Server::get_client_amt()
{
	return this->client_amt;
}

bool Server::check_nick_uniqueness(const std::string new_nick)
{
	for (size_t i = 0; i < clients.size(); ++i)
	{
		if (clients[i].get_nickname() == new_nick)
			return false;
	}
	return true;
}

void Server::debug_print_chan() const
{
	std::cout << "\n=== Channel Debug Information ===" << std::endl;
	if (channels.empty())
		std::cout << "No channels." << std::endl;
	for (size_t i = 0; i < channels.size(); i++)
	{
		const Channel &chan = channels[i];
		std::cout << "Channel #" << i + 1 << ":" << std::endl;
		std::cout << "  Name: " << chan.get_name() << std::endl;
		debug_print_chan_users(chan);
		std::cout << "----------------------------------------" << std::endl;
	}
	std::cout << "=== End Channel Debug Info ===" << std::endl;
}

const std::vector<Channel> &Server::get_vector_channels() const
{
	return channels;
}

const std::vector<Client> &Server::get_vector_clients() const
{
	return clients;
}

void Server::debug_print_chan_users(const Channel &chan) const
{
	std::cout << "  Users:" << std::endl;
	const std::vector<Client> &users = chan.get_users();
	if (users.empty())
		std::cout << "  (no users)" << std::endl;
	else
	{
		for (size_t i = 0; i < users.size(); i++)
			std::cout << "  - " << users[i].get_nickname() << std::endl;
	}
}

void Server::debug_print_ops(const Channel *channel, const std::string &context)
{
	std::cout << "=== DEBUG [" << context << "] ===" << std::endl;
	std::cout << "Channel: " << channel->get_name() << std::endl;
	const std::vector<Client> &operators = channel->get_operators();
	std::cout << "Operators (" << operators.size() << "):" << std::endl;
	for (size_t i = 0; i < operators.size(); ++i)
	{
		std::cout << " - " << operators[i].get_nickname()
				  << " (fd: " << operators[i].get_FD() << ")" << std::endl;
	}
	std::cout << "=========================" << std::endl;
}

std::string Server::get_password() const
{
	return _password;
}

void Server::schedule_close(int fd)
{
	_fdsToClose.push_back(fd);
}

std::string Server::get_servername() const
{
	return _server_name;
}

void Server::set_servername()
{
	char server_name[256];
	if (gethostname(server_name, sizeof(server_name)) != 0)
		strcpy(server_name, "localhost");
	_server_name = server_name;
}

DCCManager &Server::get_dcc_manager()
{
	return *_dcc_manager;
}

void Server::handle_dcc_error(Client &client, const std::string &error)
{
	std::string error_msg = ":" + _server_name + " ERROR :DCC: " + error + "\r\n";
	send(client.get_FD(), error_msg.c_str(), error_msg.length(), 0);
}

//void Server::handle_disconnection(int fd, const std::string &reason = "Client disconnected")
void Server::handle_disconnection(int fd, const std::string &reason)
{
	std::cout << "goes here" << std::endl;
	Client *client = find_client(fd);
	std::cout << "goes here1" << std::endl;
	if (!client)
	{
		std::cout << "no client" << std::endl;
		return;
	}
	std::cout << "goes here2" << std::endl;
	std::vector<Channel> channels = get_vector_channels(); //notify channels about disconnect
	std::cout << "goes here3" << std::endl;
	for (size_t i = 0; i < channels.size(); i++)
	{
		Channel *chan = get_channel(channels[i].get_name());
		std::cout << "goes to loop" << std::endl;
		if (chan && chan->has_user(*client))
		{
			std::cout << "goes to loop2" << std::endl;
			std::string quit_msg = ":" + client->get_nickname() + " QUIT :" + reason + "\r\n";
			const std::vector<Client> &users = chan->get_users();
			for (size_t j = 0; j < users.size(); j++)
			{
				std::cout << "goes to loop loop" << std::endl;
				if (users[j].get_FD() != fd)
					send(users[j].get_FD(), quit_msg.c_str(), quit_msg.size(), 0);
			}
			chan->remove_user(*client);
		}
	}
	close(fd);
	for (size_t i = 0; i < clients.size(); i++)
	{
		if (clients[i].get_FD() == fd)
		{
			clients.erase(clients.begin() + i);
			break;
		}
	}
}