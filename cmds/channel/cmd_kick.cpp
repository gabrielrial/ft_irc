#include "../../lib_irc.hpp"

void	cmd_kick(Server &server, RawTextLine &line, Client &client);
void	broadcast_kick(const Channel *chan, const Client &kicker, const Client *target, const std::string &reason);

void	cmd_kick(Server &server, RawTextLine &line, Client &client)
{
	std::string server_name = server.get_servername();
	if (line.get_params().empty())
	{
		err_needmoreparams(server_name, client, "KICK");
		return;
	}
	const std::vector<std::string> &params = line.get_params();
	std::string reason = "";
	if (!line.get_trailing().empty())
		reason = line.get_trailing();
	std::string chan_param = params[0]; //channels
	std::string user_param; //users
	if (params[1].empty())
	{
		err_needmoreparams(server_name, client, "KICK");
		return;
	}
	else
		user_param = params[1];
	std::istringstream channel_stream(chan_param); //splitting
	std::string channel_name;
	while (std::getline(channel_stream, channel_name, ','))
	{
		Channel *channel = server.get_channel(channel_name);
		if (!channel)
		{
			err_nosuchchannel(server_name, client, channel_name);
			return;
			//continue;
		}
		if (!channel->is_operator(client))
		{
			err_chanoprivsneeded(server_name, client, channel);
			return;
			//continue;
		}
		std::istringstream user_stream(user_param); //splitting
		std::string user_name;
		while (std::getline(user_stream, user_name, ','))
		{
			Client *target_nick = server.get_client(user_name);
			if (!target_nick)
			{
				err_nosuchnick(server_name, client, user_name, "KICK");
				return;
				//continue;
			}
			if (!channel->check_user(user_name))
			{
				err_usernotinchannel(server_name, client, channel, user_name);
				return;
				//continue;
			}
			broadcast_kick(channel, client, target_nick, reason);
			channel->remove_user(*target_nick);
			//make a check if operator, then remove from operators list
		}
	}
}

void broadcast_kick(const Channel *chan, const Client &kicker, const Client *target, const std::string &reason)
{
	if (!chan)
		return;
	std::string kick_msg = kicker.get_prefix() + " KICK " + 
							chan->get_name() + " " + target->get_nickname();
	if (!reason.empty())
		kick_msg += " :" + reason;
	kick_msg += "\r\n";
	const std::vector<Client>& users = chan->get_users();
	for (size_t i = 0; i < users.size(); ++i)
		send(users[i].get_FD(), kick_msg.c_str(), kick_msg.length(), 0);
}