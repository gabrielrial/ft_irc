#pragma once

#include <netinet/in.h> // sockaddr_in
#include <sys/socket.h> // socket functions
#include <unistd.h>		// close()
#include <string>		// std::string
#include <vector>		// std::vector

#include "../client/Client.hpp"
#include "../channel/Channel.hpp"
#include "../parser/RawTextLine.hpp"

class Server
{
private:
	uint16_t _port;
	std::string _password;
	int _socket;
	sockaddr_in _hint;
	std::vector<Client*> clients;
	std::vector<Channel> channels;
	int client_amt;
	std::string _server_name;
	std::vector<int> _fdsToClose;
	char *ip;
	//time_t _last_ping;

		void
		init_socket();
	void create_socket();
	void bind_socket();
	void start_listening();
	int prepare_fd_set(fd_set *readfds);
	void register_client();
	void handle_client_data(int fd, char *buffer, ssize_t bytes_read,
							std::string &lineBuffer);
	void process_line(int fd, const std::string &line);
	Client *find_client(int fd);
	bool check_channel(RawTextLine &line);
	void check_client(RawTextLine &line, std::vector<Client *> &client_list);
	void set_client_amt();
	int get_client_amt();
	void set_servername();
	void remove_closed_clients(std::string lineBuffer[]);
	void handle_disconnection(int fd, const std::string &reason);
	void send_ping(Server &server);

public:
	Server(uint16_t port, std::string password);
	~Server();

	std::string ping;

	// channels
	Channel *get_channel(const std::string &ch_name);
	void add_channel(const std::string &ch_name);
	const std::vector<Channel> &get_vector_channels() const;

	// clients
	Client *get_client(const std::string &cl_name);
	const std::vector<Client*> &get_vector_clients() const;

	// other utilities
	void srv_run();
	std::string get_servername() const;
	void welcome(Client client);
	bool check_nick_uniqueness(const std::string new_nick);
	std::string get_password() const;
	void schedule_close(int fd);
};
