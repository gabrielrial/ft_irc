#pragma once

#include "../lib_irc.hpp"

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <sys/socket.h>

class Client;

class Channel
{
private:
	std::string				_name;
	std::string				_topic;
	std::vector<Client*>	_users;
	std::vector<Client*>	_operators;
	bool					_mode_i;
	bool					_mode_t;
	std::string				_mode_key;
	size_t					_mode_l;
	std::vector<Client*>	_invitees;

public:
	Channel();
	Channel(const std::string &name);
	~Channel();
	Channel(const Channel &copy);
	Channel& operator=(const Channel &copy);

	// Basic channel operations
	bool		add_user(Client* client);
	bool		remove_user(Client* client);
	bool		has_user(Client* client) const;
	Client		*check_user(const std::string &cl_name) const;
	
	// Operator management
	bool		add_operator(Client* client);
	bool		rem_operator(Client* client);
	bool		is_operator(Client* client) const;
	bool		is_operator(Client* client);

	// Getters
	const std::string			&get_name() const;
	const std::string			&get_topic() const;
	const std::vector<Client*>	&get_users() const;
	const std::vector<Client*>	&get_operators() const;
	const std::vector<Client*>	&get_invitees() const;
	unsigned int			 	get_UserCount() const;
	bool						get_mode_i() const;
	bool						get_mode_t() const;
	std::string					get_mode_k() const;
	size_t						get_mode_l() const;
	std::string					get_allmode() const;

	// Setters
	void	set_topic(const std::string& topic);
	void	set_mode_i(bool value);
	void	set_mode_t(bool value);
	void	set_mode_k(std::string key);
	void	set_mode_l(size_t limit);
	void	add_to_invitees(Client *client);

};
