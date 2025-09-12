#include "Channel.hpp"

Channel::Channel(const std::string& name) : _name(name), _topic("")
{}

Channel::~Channel()
{}

bool Channel::addUser(Client client)
{
	if (hasUser(client))
		return false;
	_users.push_back(client);
	//_userModes[client] = false; // Initialize with no special modes
	return true;
}

//bool Channel::removeUser(int client_fd)
//{
//	std::vector<int>::iterator it = std::find(_users.begin(), _users.end(), client_fd);
//	if (it == _users.end())
//		return false;
//	
//	_users.erase(it);
//	_userModes.erase(client_fd);
//	
//	// Also remove from operators if they were one
//	removeOperator(client_fd);
//	return true;
//}
//
bool Channel::hasUser(Client client) const
{
	return std::find(_users.begin(), _users.end(), client) != _users.end();
}

// bool Channel::addOperator(int client_fd)
// {
// 	if (!hasUser(client_fd) || isOperator(client_fd))
// 		return false;
// 	_operators.push_back(client_fd);
// 	return true;
// }
//
//bool Channel::removeOperator(int client_fd)
//{
//	std::vector<int>::iterator it = std::find(_operators.begin(), _operators.end(), client_fd);
//	if (it == _operators.end())
//		return false;
//	
//	_operators.erase(it);
//	return true;
//}
//
//bool Channel::isOperator(int client_fd) const
//{
//	return std::find(_operators.begin(), _operators.end(), client_fd) != _operators.end();
//}

const std::string& Channel::getName() const
{
	return _name;
}

// const std::string& Channel::getTopic() const
// {
// 	return _topic;
// }

const std::vector<Client>& Channel::getUsers() const
{
	return _users;
}
//
//const std::vector<int>& Channel::getOperators() const
//{
//	return _operators;
//}

// void Channel::setTopic(const std::string& topic)
// {
// 	_topic = topic;
// }

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

void	Channel::names_list(const Channel *chan, const Client &client) const
{
	if (!chan)
		return;
	const std::vector<Client> &users = chan->getUsers();
	std::string user_list;
	for (size_t i = 0; i < users.size(); ++i)
	{
		if (i > 0)
			user_list += " ";
		user_list += users[i].getNickname();
	}
	std::string names_reply = ":localhost 353 " + client.getNickname() + " = " + 
							chan->getName() + " :" + user_list + "\r\n"; //RPL_NAMREPLY
	std::string end_names = ":localhost 366 " + client.getNickname() + " " + 
						chan->getName() + " :End of /NAMES list.\r\n"; //RPL_ENDOFNAMES
	for (size_t i = 0; i < users.size(); ++i)
	{
		const Client &target = users[i];
		send(target.getFd(), names_reply.c_str(), names_reply.length(), 0);
		send(target.getFd(), end_names.c_str(), end_names.length(), 0);
	}
}