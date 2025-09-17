#pragma once

#include <netinet/in.h> // sockaddr_in
#include <sys/socket.h> // socket functions
#include <unistd.h>		// close()
#include <string>		// std::string
#include <vector>		// std::vector

#include "../client/Client.hpp"
#include "../channel/Channel.hpp"
#include "../parser/Parser.hpp"

class Server
{
private:
	uint16_t _port;
	std::string _password;
	int _socket;
	sockaddr_in _hint;
	std::vector<Client> clients;
	std::vector<Channel> channels;

	void init_socket();
	void create_socket();
	void bind_socket();
	void start_listening();

	int prepare_fd_set(fd_set *readfds);
	std::vector<int> _fdsToClose;

	void register_client();
	Client *find_client(int fd);

	void process_line(int fd, const std::string &line);
	void handle_client_data(int fd, char *buffer, ssize_t bytes_read, std::string &lineBuffer);

public:
	Server(uint16_t port, std::string password);
	~Server();

	void check_client(RawTextLine &line, std::vector<Client *> &client_list);
	bool check_channel(RawTextLine &line);
	void welcome(Client client);

	void add_channel(const std::string &ch_name);
	Channel *get_channel(const std::string &ch_name);

	const std::vector<Channel> &get_vector_channels() const;
	const std::vector<Client> &get_vector_clients() const;

	std::string get_password() const;
	void remove_closed_clients(std::string lineBuffer[]);
	void debug_print_chan() const;
	void debug_print_chan_users(const Channel &chan) const;

	void srv_run();
	void schedule_close(int fd);
};
