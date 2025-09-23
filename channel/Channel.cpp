#include "Channel.hpp"

Channel::Channel() : 
	_name(""),
	_topic(""),
	_mode_i(false),
	_mode_t(true),
	_mode_key(""),
	_mode_l(20)
{}

Channel::Channel(const std::string &name) : 
	_name(name),
	_topic(""),
	_mode_i(false),
	_mode_t(true),
	_mode_key(""),
	_mode_l(20)
{}

Channel::~Channel()
{}

Channel::Channel(const Channel &copy)
{
	*this = copy;
}

Channel& Channel::operator=(const Channel &copy)
{
	if (this != &copy) 
	{
		_name = copy._name;
		_topic = copy._topic;
		_users = copy._users;
		_operators = copy._operators;
		_mode_i = copy._mode_i;
		_mode_t = copy._mode_t;
		_mode_key = copy._mode_key;
		_mode_l = copy._mode_l;
	}
	return *this;
}

bool	Channel::add_user(Client client)
{
	if (has_user(client))
		return false;
	_users.push_back(client);
	//_userModes[client] = false; // Initialize with no special modes
	return true;
}

bool	Channel::remove_user(Client client)
{
	std::vector<Client>::iterator it = std::find(_users.begin(), _users.end(), client);
	if (it == _users.end())
		return false;
	_users.erase(it);
	//_userModes.erase(client_fd);
	//removeOperator(client_fd);
	return true;
}
//
bool	Channel::has_user(Client client) const
{
	return std::find(_users.begin(), _users.end(), client) != _users.end();
}

bool Channel::add_operator(Client client)
{
	if (!has_user(client) || is_operator(client))
		return false;
	_operators.push_back(client);
	return true;
}

bool Channel::rem_operator(Client client)
{
	std::vector<Client>::iterator it = std::find(_operators.begin(), _operators.end(), client);
	if (it == _operators.end())
		return false;
	
	_operators.erase(it);
	return true;
}

bool Channel::is_operator(Client client) const
{
	return std::find(_operators.begin(), _operators.end(), client) != _operators.end();
}

const	std::string &Channel::get_name() const
{
	return _name;
}

const	std::string &Channel::get_topic() const
{
	return _topic;
}

unsigned int Channel::get_UserCount() const
{
	return _users.size();
}

const std::vector<Client>& Channel::get_users() const
{
	return _users;
}

const std::vector<Client>& Channel::get_operators() const
{
	return _operators;
}

const std::vector<Client>& Channel::get_invitees() const
{
	return _invitees;
}

bool Channel::get_mode_i() const
{
	return _mode_i;
}

bool Channel::get_mode_t() const
{
	return _mode_t;
}

std::string Channel::get_mode_k() const
{
	return _mode_key;
}

size_t Channel::get_mode_l() const
{
	return _mode_l;
}

std::string Channel::get_allmode() const
{
	std::string modes = "+";
	if (this->get_mode_i())
		modes += "i";
	if (this->get_mode_t())
		modes += "t";
	if (!this->get_mode_k().empty())
		modes += "k";
	if (this->get_mode_l())
	{
		std::stringstream ss;
		ss << "l " << this->get_mode_l();
		modes += ss.str();
	}
	return modes;
}

void	Channel::set_topic(const std::string &topic)
{
	_topic = topic;
}

void	Channel::set_mode_i(bool value)
{
	_mode_i = value;
}

void	Channel::set_mode_t(bool value)
{
	_mode_t = value;
}

void	Channel::set_mode_k(std::string key)
{
	_mode_key = key;
}

void	Channel::set_mode_l(size_t limit)
{
	_mode_l = limit;
}

void	Channel::add_to_invitees(Client *client)
{
	_invitees.push_back(*client);
}

//void Channel::broadcast(const std::string& message, int sender_fd)
//{
//	for (std::vector<int>::const_iterator it = _users.begin(); it != _users.end(); ++it)
//	{
//		if (*it != sender_fd) // Don't send back to sender if sender_fd is provided
//		{
//			send(*it, message.c_str(), message.length(), 0);
//		}
//	}
//}