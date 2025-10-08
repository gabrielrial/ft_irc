#include "../../lib_irc.hpp"

void cmd_names(Server &server, RawTextLine &line, Client &client);
bool valid_param(RawTextLine &line);
bool user_in_channel(const Channel *channel, Client &client);
void	send_user_inchan(Server &server, const Channel &channel, Client &client);
void	send_user_notinchan(Server &server, Client &client);
bool	check_user_in_allchannel(const Client &client, const std::vector<Channel> &channels);

void cmd_names(Server &server, RawTextLine &line, Client &client)
{
	if (!valid_param(line))
		return;

	if (line.get_params().empty() || line.get_params()[0] == server.get_servername())
	{
		const std::vector<Channel> &channels = server.get_vector_channels();
		for (size_t i = 0; i < channels.size(); i++)
		{
			const Channel &channel = channels[i];
			if (user_in_channel(&channel, client))
				send_user_inchan(server, channel, client);
		}
		send_user_notinchan(server, client);
		return;
	}

	size_t channel_size = line.get_sep_params().size();

	for (size_t c = 0; c < channel_size; c++)
	{
		Channel *channel = server.get_channel(line.get_sep_params()[c]);
		if (!channel)
		{
			err_nosuchchannel(server.get_servername(), client, line.get_sep_params()[c]);
			continue;
		}
		if (!user_in_channel(channel, client))
		{
			err_notonchannel(server.get_servername(), client, channel);
			continue;
		}

		std::vector<Client*> client_list = channel->get_users();

		std::string rpl_namreply = ":" + server.get_servername() + " 353 " +
								   client.get_nickname() + " = " + channel->get_name() + " :";

		std::string names_list;
		// we send the normal users first.
		for (size_t i = 0; i < client_list.size(); i++)
		{
			std::string client_name = (*client_list[i]).get_nickname();

			if (channel->is_operator(client_list[i]))
				client_name = "@" + client_name;

			if ((rpl_namreply + names_list + client_name).size() < 510)
			{
				if (!names_list.empty())
					names_list += " ";
				names_list += client_name;
			}
			else
			{
				std::string msg = rpl_namreply + names_list + CRLF;
				send(client.get_FD(), msg.c_str(), msg.size(), 0);
				names_list = client_name;
				continue;
			}
			if (client_list.size() - 1 == i)
			{
				std::string msg = rpl_namreply + names_list + CRLF;
				send(client.get_FD(), msg.c_str(), msg.size(), 0);
				names_list = client_name;
			}
		}

		std::string endmsg = ":" + server.get_servername() + " 366 " +
							 client.get_nickname() + " " + channel->get_name() +
							 " :End of NAMES list" + CRLF;
		send(client.get_FD(), endmsg.c_str(), endmsg.size(), 0);
	}
}

/* valid_param()
 *
 * Check for valid parameters.
 * It will only works in this case.
 *
 * /NAME [<#channel><,#channel>]
 * */

bool valid_param(RawTextLine &line)
{
	if (line.get_params().size() > 1)
	{
		//std::cout << "rerror" << std::endl;
		return false;
	}
	else if (!line.get_trailing().empty())
	{
		//std::cout << "rerror" << std::endl;
		return false;
	}
	return true;
}

/* valid_param()
 *
 * Checks if we belong to the channel or if we were invited to.
 * We should be on the channel user list or in invitiees list.
 * */

bool user_in_channel(const Channel *channel, Client &client)
{
	std::vector<Client*> user_list = channel->get_users();
	std::vector<Client*> invitiees_list = channel->get_invitees();

	size_t count = user_list.size();
	size_t count_i = invitiees_list.size();

	for (size_t i = 0; i < count; i++)
	{
		if (client.get_nickname() == (*user_list[i]).get_nickname())
			return true;
		if (i < count_i && client.get_nickname() == invitiees_list[i]->get_nickname())
			return true;
	}

	return false;
}

void	send_user_inchan(Server &server, const Channel &channel, Client &client)
{
	std::string names_list;
	const std::vector<Client*> &users = channel.get_users();
	for (size_t i = 0; i < users.size(); i++)
	{
		std::string user_name = (*users[i]).get_nickname();
		if (channel.is_operator(users[i]))
			user_name = "@" + user_name;
		if (!names_list.empty())
			names_list += " ";
		names_list += user_name;
	}
	if (!names_list.empty())
	{
		rpl_namreply(server.get_servername(), client, channel.get_name(), names_list);
		rpl_endofnames(server.get_servername(), client, channel.get_name());
	}
}

void	send_user_notinchan(Server &server, Client &client)
{
	std::string names_list;
	const std::vector<Client> &all_clients = server.get_vector_clients();
	const std::vector<Channel> &channels = server.get_vector_channels();
	for (size_t i = 0; i < all_clients.size(); i++)
	{
		if (!check_user_in_allchannel(all_clients[i], channels))
		{
			if (!names_list.empty())
				names_list += " ";
			names_list += all_clients[i].get_nickname();
		}
	}
	if (!names_list.empty())
	{
		rpl_namreply(server.get_servername(), client, "*", names_list);
		rpl_endofnames(server.get_servername(), client, "*");
	}
}

bool	check_user_in_allchannel(const Client &client, const std::vector<Channel> &channels)
{
	for (size_t i = 0; i < channels.size(); i++)
	{
		const std::vector<Client*> &users = channels[i].get_users();
		for (size_t j = 0; j < users.size(); j++)
		{
			if ((*users[j]).get_nickname() == client.get_nickname())
				return true;
		}
	}
	return false;
}