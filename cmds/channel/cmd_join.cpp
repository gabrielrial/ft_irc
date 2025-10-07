#include "../../lib_irc.hpp"

void	add_client_channel(Client &client, Channel &channel, bool empty_channel, std::string chan_name)
{
	channel.add_user(&client);
	if (empty_channel)
		channel.add_operator(&client);
	std::string joinMsg = ":" + client.get_nickname() +  //works with hexchat
								" JOIN " + chan_name + "\r\n";
	// std::string joinMsg = ":" + client.get_prefix() +  //doesnt work with hexchat
	// 					" JOIN " + chan_name + "\r\n";
	send(client.get_FD(), joinMsg.c_str(), joinMsg.length(), 0);
}

void	cmd_join(Server &server, RawTextLine &line, Client &client)
{
	std::string server_name = server.get_servername();
	if (line.get_params().empty())
	{
		err_needmoreparams(server_name, client, "JOIN");
		return;
	}
	const std::vector<std::string> &params = line.get_params();
	std::string first_param = params[0]; //channels
	std::string key; // keyword
	if (params.size() > 1)
		key = params[1];
	size_t start = 0;
	size_t end = 0;
	while (start < first_param.length())
	{
		end = first_param.find(',', start);
		if (end == std::string::npos)
			end = first_param.length();
		std::string channel_name = first_param.substr(start, end - start);
		if (channel_name[0] != '#' && channel_name[0] != '&' &&
			channel_name[0] != '+' && channel_name[0] != '!')
			//channel_name = "#" + channel_name; //must be ERR_BADCHANMASK???
		{
			err_badchanmask(server_name, client, channel_name);
			start = end + 1;
			continue;
		}
		server.add_channel(channel_name);
		Channel *channel = server.get_channel(channel_name);
		bool empty_channel = (channel->get_UserCount() == 0);
		if (!channel->get_mode_k().empty() && (key != channel->get_mode_k())) //check key
		{
			err_badchannelkey(server_name, client, channel);
			start = end + 1;
			continue;
		}
		if (channel->get_mode_l() > 0 && channel->get_UserCount() >= channel->get_mode_l()) //check limit
		{
			err_channelisfull(server_name, client, channel);
			start = end + 1;
			continue;
		}
		if (channel->get_mode_i() == false)
		{
			add_client_channel(client, *channel, empty_channel, channel_name);
			cmd_names_join(server, channel_name);
		}
		else // invite-only
		{
			const std::vector<Client*> &invitees = channel->get_invitees(); // return vector<Client*> or references
			bool allowed = false;
			for (size_t i = 0; i < invitees.size(); ++i)
			{
				if (client.get_nickname() == invitees[i]->get_nickname())
				{
					allowed = true;
					break;
				}
			}
			if (allowed == true)
			{
				add_client_channel(client, *channel, empty_channel, channel_name);
				cmd_names_join(server, channel_name);
			}
			else
			{
				std::string err = ":" + server_name + " 473 " + client.get_nickname() +
								  " " + channel_name + " :Cannot join channel (+i)\r\n";
				send(client.get_FD(), err.c_str(), err.length(), 0);
			}
		}
		start = end + 1;
	}
}

