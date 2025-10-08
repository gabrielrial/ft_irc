#include "../../lib_irc.hpp"

void	cmd_topic(Server &server, RawTextLine &line, Client &client);
void	topic_query(Channel *channel, Client &client, std::string &server_name);
void	topic_change(Channel *channel, Client &client, std::string &server_name, const std::string &new_topic);

void	cmd_topic(Server &server, RawTextLine &line, Client &client)
{
	std::string server_name = server.get_servername();
	if (line.get_params().empty())
	{
		err_needmoreparams(server_name, client, "TOPIC");
		return;
	}
	const std::vector<std::string> &sep_params = line.get_sep_params();
	std::string channel_name = sep_params[0];
	Channel *channel = server.get_channel(channel_name);
	if (!channel)
	{
		err_nosuchchannel(server_name, client, channel_name);
		return;
	}
	else if (!channel->has_user(&client))
	{
		err_notonchannel(server_name, client, channel);
		return;
	}
	if (line.get_params().size() == 1 && line.get_trailing().empty())
		topic_query(channel, client, server_name);
	else
		topic_change(channel, client, server_name, line.get_trailing());
}

void	topic_query(Channel *channel, Client &client, std::string &server_name)
{
	std::string topic = channel->get_topic();
	if (topic.empty())
		rpl_notopic(server_name, client, channel->get_name());
	else
		rpl_topic(server_name, client, channel->get_name(), topic);
}

void	topic_change(Channel *channel, Client &client, std::string &server_name, const std::string &new_topic)
{
	if (!channel->get_mode_t())
	{
		if (!channel->is_operator(&client))
		{
			err_chanoprivsneeded(server_name, client, channel);
			return;
		}
	}
	channel->set_topic(new_topic);
	std::string announce = client.get_prefix() + " TOPIC " + channel->get_name() + 
							" :" + new_topic + "\r\n";
	const std::vector<Client*>& users = channel->get_users();
	for (size_t i = 0; i < users.size(); ++i)
		send((*users[i]).get_FD(), announce.c_str(), announce.length(), 0);
}
