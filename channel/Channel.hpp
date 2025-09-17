#pragma once

#include "../lib_irc.hpp"

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <sys/socket.h>
class Client;

class Channel {
private:
	std::string			_name;
	std::string			_topic;
	std::vector<Client>	_users;		// Store client file descriptors
	std::vector<Client>	_operators;	// Store operator client file descriptors

public:
	Channel();
	Channel(const std::string& name);
	~Channel();

	// Basic channel operations
	bool		add_user(Client client);
	bool		remove_user(Client client);
	bool		has_user(Client client) const;
	
	// Operator management
	//bool		addOperator(int client_fd);
	//bool		removeOperator(int client_fd);
	//bool		isOperator(int client_fd) const;

	// Getters
	const std::string			&get_name() const;
	const std::string			&get_topic() const;
	const std::vector<Client>	&get_users() const;
	//const std::vector<int>	&getOperators() const;
	unsigned int			 	get_UserCount() const;

	// Setters
	void	set_topic(const std::string& topic);

	// Broadcast message to all users in channel
	//void	broadcast(const std::string& message, int sender_fd = -1);
};
