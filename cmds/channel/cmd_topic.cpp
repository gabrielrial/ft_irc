#include "../../lib_irc.hpp"

void	cmd_topic(Server &server, RawTextLine &line, Client &client);
int		check_topic_params(RawTextLine &line, Client &client, std::string server_name);
int		check_topic_channel(Server &server, Client &client, 
							std::string channel_name, std::string server_name);
void	topic_queryandbroadcast(Server &server, Client &client, 
							const std::vector<std::string> &params, 
							RawTextLine &line,
							std::string channel_name , std::string server_name);
void	broadcast_topic(Channel *chan, const Client &client, RawTextLine &line,
						std::string channel_name, std::string server_name);

void	cmd_topic(Server &server, RawTextLine &line, Client &client)
{
	char server_name[256];
	if (gethostname(server_name, sizeof(server_name)) != 0)
		strcpy(server_name, "localhost");
	if (check_topic_params(line, client, server_name) == 1)
		return;
	const std::vector<std::string> &sep_params = line.get_sep_params();
	std::string channel_name = sep_params[0];
	if (check_topic_channel(server, client, channel_name, server_name) == 1)
		return;
	const std::vector<std::string> &params = line.get_params();
	topic_queryandbroadcast(server, client, params, line, channel_name, server_name);
}

int	check_topic_params(RawTextLine &line, Client &client, std::string server_name)
{
	if (line.get_params().empty())
	{
		std::string err_needmoreparams = ":" + std::string(server_name) + " 461 " + 
				client.get_nickname() + " TOPIC :Not enough parameters\r\n";
		send(client.get_FD(), err_needmoreparams.c_str(), err_needmoreparams.length(), 0);
		return 1;
	}
	return 0;
}

int	check_topic_channel(Server &server, Client &client, 
						std::string channel_name, std::string server_name)
{
	Channel *channel = server.get_channel(channel_name);
	if (!channel)
	{
		std::string err_nosuchchannel = ":" + std::string(server_name) + " 403 " + 
				client.get_nickname() + " " + channel_name + " :No such channel\r\n";
		send(client.get_FD(), err_nosuchchannel.c_str(), err_nosuchchannel.size(), 0);
		return 1;
	}
	else if (!channel->has_user(client))
	{
		std::string err_notonchannel = ":" + std::string(server_name) + " 442 " + 
				client.get_nickname() + " " + channel_name + " :You're not on that channel\r\n";
		send(client.get_FD(), err_notonchannel.c_str(), err_notonchannel.size(), 0);
		return 1;
	}
	return 0;
}

void	topic_queryandbroadcast(Server &server, Client &client, 
							const std::vector<std::string> &params, 
							RawTextLine &line,
							std::string channel_name , std::string server_name)
{
	Channel *channel = server.get_channel(channel_name);
	if (params.size() == 1 && line.get_trailing().empty()) 
	{
		std::string topic = channel->get_topic();
		if (topic.empty())
		{
			std::string rpl_notopic = ":" + std::string(server_name) + " 331 " + 
					client.get_nickname() + " " + channel_name + " :No topic is set\r\n";
			send(client.get_FD(), rpl_notopic.c_str(), rpl_notopic.size(), 0);
		}
		else
		{
			std::string rpl_topic = ":" + std::string(server_name) + " 332 " + 
					client.get_nickname() + " " + channel_name + " :" + topic + "\r\n";
			send(client.get_FD(), rpl_topic.c_str(), rpl_topic.size(), 0);
		}
	}
	else
		broadcast_topic(channel, client, line, channel_name, server_name);
}

void	broadcast_topic(Channel *chan, const Client &client, RawTextLine &line,
						std::string channel_name, std::string server_name)
{
	std::string new_topic = line.get_trailing();
	if (!chan->is_operator(client))
	{
		std::string err_chanoprivsneeded = ":" + server_name + " 482 " + 
							client.get_nickname() + " " + channel_name + 
							" :You're not channel operator\r\n";
		send(client.get_FD(), err_chanoprivsneeded.c_str(), err_chanoprivsneeded.length(), 0);
		return;
	}
	chan->set_topic(new_topic);
	//std::string announce = client.get_nickname() + " TOPIC " + channel_name + " :" + new_topic + "\r\n";
	std::string announce = client.get_prefix() + " TOPIC " + channel_name + " :" + new_topic + "\r\n";
	const std::vector<Client>& users = chan->get_users();
	for (size_t i = 0; i < users.size(); ++i)
		send(users[i].get_FD(), announce.c_str(), announce.length(), 0);
}