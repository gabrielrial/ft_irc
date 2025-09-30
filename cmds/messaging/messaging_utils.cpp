#include "../../lib_irc.hpp"

void send_message_to_channel(Channel *channel, Client &sender,
							 const std::string &targetName, const std::string &message,
							 const std::string &command, bool sendErrors)
{
	if (!channel)
	{
		if (sendErrors)
		{
			std::string msg = ":localhost 403 " + sender.get_nickname() + " " + targetName + " :No such channel\r\n";
			send(sender.get_FD(), msg.c_str(), msg.size(), 0);
		}
		return;
	}

	const std::vector<Client> &users = channel->get_users();
	for (size_t j = 0; j < users.size(); ++j)
	{
		if (users[j].get_FD() == sender.get_FD())
			continue;

		std::string msg = sender.get_prefix() + " " + command + " " + targetName + " :" + message + "\r\n";
		std::cout << msg << std::endl;
		send(users[j].get_FD(), msg.c_str(), msg.size(), 0);
	}
}

void send_message_to_user(Server &server, Client &sender,
						  const std::string &targetName, const std::string &message,
						  const std::string &command, bool sendErrors)
{
	Client *target = server.get_client(targetName);
	if (!target)
	{
		if (sendErrors)
		{
			std::string msg = ":localhost 401 " + sender.get_nickname() + " " + targetName + " :No such nick\r\n";
			send(sender.get_FD(), msg.c_str(), msg.size(), 0);
		}
		return;
	}

	std::string msg = sender.get_prefix() + " " + command + " " + target->get_nickname() + " :" + message + "\r\n";
	std::cout << msg << std::endl;
	send(target->get_FD(), msg.c_str(), msg.size(), 0);
}

void send_message(Server &server, Client &sender,
				  const std::vector<std::string> &targets, const std::string &message,
				  const std::string &command, bool sendErrors)
{
	if (targets.empty())
	{
		if (sendErrors)
		{
			std::string msg = ":localhost 411 " + sender.get_nickname() + " :No recipient given (" + command + ")\r\n";
			send(sender.get_FD(), msg.c_str(), msg.size(), 0);
		}
		return;
	}

	if (message.empty())
	{
		if (sendErrors)
		{
			std::string msg = ":localhost 412 " + sender.get_nickname() + " :No text to send\r\n";
			send(sender.get_FD(), msg.c_str(), msg.size(), 0);
		}
		return;
	}

	for (size_t i = 0; i < targets.size(); ++i)
	{
		if (targets[i][0] == '#' || targets[i][0] == '$' || targets[i][0] == '!' || targets[i][0] == '&')
			send_message_to_channel(server.get_channel(targets[i]), sender, targets[i], message, command, sendErrors);
		else
			send_message_to_user(server, sender, targets[i], message, command, sendErrors);
	}
}

std::vector<std::string> get_target(RawTextLine &line)
{
	std::vector<std::string> targets;
	if (line.get_params().empty())
		return targets;
	std::stringstream ss(line.get_params()[0]);
	std::string token;
	while (std::getline(ss, token, ','))
		targets.push_back(token);
	return targets;
}