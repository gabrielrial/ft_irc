#pragma once

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <sys/socket.h>

class Channel {
private:
	std::string			_name;
	std::string			_topic;
	std::vector<Client>	_users;		// Store client file descriptors
	std::vector<Client>	_operators;	// Store operator client file descriptors
	//std::map<Client, bool>	_userModes;	// Store user modes (e.g., voice status)

public:
	Channel(const std::string& name);
	~Channel();

	// Basic channel operations
	bool		addUser(int client_fd);
	bool		removeUser(int client_fd);
	bool		hasUser(int client_fd) const;
	
	// Operator management
	bool		addOperator(int client_fd);
	bool		removeOperator(int client_fd);
	bool		isOperator(int client_fd) const;

	// Getters
	const std::string&		  getName() const;
	const std::string&		  getTopic() const;
	const std::vector<int>&	 getUsers() const;
	const std::vector<int>&	 getOperators() const;

	// Setters
	void	setTopic(const std::string& topic);

	// Broadcast message to all users in channel
	void	broadcast(const std::string& message, int sender_fd = -1);
};
