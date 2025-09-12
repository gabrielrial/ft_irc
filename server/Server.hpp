#pragma once

#include <netinet/in.h>   // sockaddr_in
#include <sys/socket.h>   // socket functions
#include <unistd.h>       // close()
#include <string>         // std::string
#include <vector>         // std::vector

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

	int prepareFdSet(fd_set *readfds);
	std::vector<int> _fdsToClose;

	void acceptNewClient();
	Client* findClient(int fd);
	void	welcome(Client client);

	void processLine(int fd, const std::string &line);
	void handleClientData(int fd, char *buffer, ssize_t bytes_read, std::string &lineBuffer);

	
	public:
		Server(uint16_t port, std::string password);
		~Server();
	
		void check_client(RawTextLine &line, std::vector<Client*> &client_list);
		bool check_channel(RawTextLine &line);

		void		addChannel(const std::string &ch_name);
		Channel*	getChannel(const std::string &ch_name);
		std::string getPassword() const;
		void 		removeClosedClients(std::string lineBuffer[]);
		void		debugPrintChan() const;

	void srv_run();
};
