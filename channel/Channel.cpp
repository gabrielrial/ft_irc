#include "Channel.hpp"

Channel::Channel() : 
	_name(""),
	_topic("")
{}

Channel::Channel(const std::string &name) : 
	_name(name),
	_topic("")
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

//const std::vector<int>& Channel::getOperators() const
//{
//	return _operators;
//}

void	Channel::set_topic(const std::string &topic)
{
	_topic = topic;
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