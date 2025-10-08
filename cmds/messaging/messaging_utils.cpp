#include "../../lib_irc.hpp"

void send_message_to_channel(Channel *channel, Client &sender,
							 const std::string &targetName, const std::string &message,
							 const std::string &command, bool sendErrors, Server &server)
{
	if (!channel)
	{
		if (sendErrors)
			err_nosuchchannel(server.get_servername(), sender, targetName);
		return;
	}

	const std::vector<Client*> &users = channel->get_users();
	for (size_t j = 0; j < users.size(); ++j)
	{
		if ((*users[j]).get_FD() == sender.get_FD())
			continue;

		std::string msg = sender.get_prefix() + " " + command + " " + targetName + " :" + message + "\r\n";
		std::cout << msg << std::endl;
		send((*users[j]).get_FD(), msg.c_str(), msg.size(), 0);
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
			err_nosuchnick(server.get_servername(), sender, targetName, "PRIVMSG");
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
			err_norecipient(server.get_servername(), sender, command);
		return;
	}

	if (message.empty())
	{
		if (sendErrors)
			err_notexttosend(server.get_servername(), sender);
		return;
	}

	for (size_t i = 0; i < targets.size(); ++i)
	{
		if (targets[i][0] == '#' || targets[i][0] == '$' || targets[i][0] == '!' || targets[i][0] == '&')
			send_message_to_channel(server.get_channel(targets[i]), sender, targets[i], message, command, sendErrors, server);
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