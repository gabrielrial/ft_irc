#include "Server.hpp"

Server::Server(uint16_t port, std::string password)
	: _port(port), _password(password), _socket(-1), client_amt(0)
{
	set_servername();
	try
	{
		init_socket();
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
	close (_socket);
}

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

	// make server socket non-blocking

	if (fcntl(_socket, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("fcntl failed");
}

void Server::bind_socket()
{
	memset(&_hint, 0, sizeof(_hint));
	_hint.sin_family = AF_INET;				 // IPv4
	_hint.sin_port = htons(_port);			 // convert port to network byte order
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
	int	ping = 0;
	char buffer[BUFFER_SIZE];
	std::string lineBuffer[FD_SETSIZE]; // one buffer per fd

	while (g_running)
	{
		if (ping > 20)
		{
			send_ping(*this);
			ping = 0;
		}
		remove_closed_clients(lineBuffer);
		fd_set readfds;
		int max_fd = prepare_fd_set(&readfds);

		int activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);
		ping++;
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
				if (bytes_read <= 0)
				{
					if (bytes_read < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
					{
						++i;
						continue;
					}
					if (bytes_read == 0)
						handle_disconnection(fd, "Connection closed");
					else
						handle_disconnection(fd, strerror(errno));
					continue;
				}
				buffer[bytes_read] = '\0';
				handle_client_data(fd, buffer, bytes_read, lineBuffer[fd]);
			}
			++i;
		}
		remove_closed_clients(lineBuffer);
	}
}

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
	if (fcntl(new_fd, F_SETFL, O_NONBLOCK) == -1)
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

void Server::handle_client_data(int fd, char *buffer, ssize_t bytes_read, std::string &lineBuffer)
{
	buffer[bytes_read] = '\0';
	lineBuffer.append(buffer);

	size_t pos;
	while ((pos = lineBuffer.find("\r\n")) != std::string::npos)
	{
		std::string line = lineBuffer.substr(0, pos);
		lineBuffer.erase(0, pos + 2);
		process_line(fd, line);
	}
}

void Server::process_line(int fd, const std::string &line)
{
	Client *client = find_client(fd);

	std::cout << "RAW (fd=" << fd << ") >>> " << line << std::endl;
	RawTextLine parsed(line);
	run_cmds(*this, parsed, *client);
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

void Server::check_client(RawTextLine &line, std::vector<Client *> &client_list)
{
	for (size_t i = 0; i < clients.size(); i++)
	{
		for (size_t a = 0; a < line.get_sep_params().size(); a++)
		{
			if (line.get_sep_params()[a] == clients[i].get_nickname())
				client_list.push_back(&clients[i]);
		}
	}
}

void Server::set_client_amt()
{
	this->client_amt++;
}

int Server::get_client_amt()
{
	return this->client_amt;
}

void Server::set_servername()
{
	char server_name[256];
	if (gethostname(server_name, sizeof(server_name)) != 0)
		strcpy(server_name, "localhost");
	_server_name = server_name;
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

void	Server::handle_disconnection(int fd, const std::string &reason)
{
	Client *client = find_client(fd);
	if (!client)
		return;
	std::vector<Channel> channels = get_vector_channels();
	for (size_t i = 0; i < channels.size(); i++)
	{
		Channel *chan = get_channel(channels[i].get_name());
		if (chan && chan->has_user(client))
		{
			std::string quit_msg = ":" + client->get_nickname() + " QUIT :" + reason + "\r\n";
			const std::vector<Client*> &users = chan->get_users();
			for (size_t j = 0; j < users.size(); j++)
			{
				if ((*users[j]).get_FD() != fd)
					send((*users[j]).get_FD(), quit_msg.c_str(), quit_msg.size(), 0);
			}
			chan->remove_user(client);
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

// like check_channel but returns the channel
Channel *Server::get_channel(const std::string &name)
{
	std::string chanel_name = name;
	if (name[0] != '#' && (name[0] != '&' && name[0] != '!' && name[0] != '+'))
		chanel_name = "#" + name;

	for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		if (it->get_name() == chanel_name)
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

const std::vector<Channel> &Server::get_vector_channels() const
{
	return channels;
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

const std::vector<Client> &Server::get_vector_clients() const
{
	return clients;
}

std::string Server::get_servername() const
{
	return _server_name;
}

void Server::welcome(Client client)
{
		std::string welcome = ":" + this->get_servername() + " 001 " + client.get_nickname() +
								" :Welcome to " + this->get_servername() + ", " + client.get_nickname() + "!" + "\r\n";
		send(client.get_FD(), welcome.c_str(), welcome.size(), 0);
		this->set_client_amt();
		return;
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

std::string Server::get_password() const
{
	return _password;
}

void Server::schedule_close(int fd)
{
	_fdsToClose.push_back(fd);
}

void Server::send_ping(Server &server)
{
	std::string ping_str = "MONTEVIDEO";
	ping = "MONTEVIDEO";

	for (size_t i = 0; i < server.clients.size(); i++)
	{
		if (server.clients[i].get_pong() == false)
			handle_disconnection(server.clients[i].get_FD(), "Inactivity");
		else
			server.clients[i].set_pong();
		std::string msg = "PING :" + ping_str + "\r\n";
		send(server.clients[i].get_FD(), msg.c_str(), msg.size(), 0);
	}
}