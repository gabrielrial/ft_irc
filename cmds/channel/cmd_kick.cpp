#include "../../lib_irc.hpp"

void	cmd_kick(Server &server, RawTextLine &line, Client &client);

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
		}
		if (!channel->is_operator(client))
		{
			err_chanoprivsneeded(server_name, client, channel);
			return;
		}
		std::istringstream user_stream(user_param); //splitting
		std::string user_name;
		while (std::getline(user_stream, user_name, ','))
		{
			if (!channel->check_user(user_name))
			{
				err_usernotinchannel(server_name, client, channel, user_name);
				return;
			}
			broadcast_part(channel, client, reason);
			channel->remove_user(channel->);
		}
	}
}