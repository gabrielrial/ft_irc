#include "../../lib_irc.hpp"

void	cmd_mode(Server &server, RawTextLine &line, Client &client);
void	change_mode(Server &server, Client &client, Channel *channel, 
				const std::vector<std::string> &params, std::string server_name);

void cmd_mode(Server &server, RawTextLine &line, Client &client)
{
	std::string server_name = server.get_servername();
	if (line.get_params().empty())
	{
		err_needmoreparams(server_name, client, "MODE");
		return;
	}
	const std::vector<std::string> &params = line.get_params();
	std::string channel_name = params[0];
	Channel *channel = server.get_channel(channel_name);
	if (!channel)
	{
		err_nosuchchannel(server_name, client, channel_name);
		return;
	}
	else if (!channel->has_user(client))
	{
		err_notonchannel(server_name, client, channel);
		return;
	}
	if (params.size() == 1)
	{
		std::string modestring = channel->get_allmode();
		rpl_channelmodeis(server_name, client, channel_name, modestring);
		return;
	}
	if (!channel->is_operator(client))
	{
		err_chanoprivsneeded(server_name, client, channel);
		return;
	}
	else
		change_mode(server, client, channel, params, server_name);
}

void change_mode(Server &server, Client &client, Channel *channel,
				const std::vector<std::string> &params, std::string server_name)
{
	std::string mode_changes;
	std::vector<std::string> mode_params;
	bool adding = true;
	for (size_t i = 0; i < params[1].size(); ++i)
	{
		char mode = params[1][i];
		switch (mode)
		{
			case '+':
				adding = true;
				break;
			case '-':
				adding = false;
				break;
			case 'i':
				channel->set_mode_i(adding);
				mode_changes += (adding ? "+" : "-");
				mode_changes += "i";
				break;
			case 't':
				channel->set_mode_t(adding);
				mode_changes += (adding ? "+" : "-");
				mode_changes += "i";
				break;
			case 'k':
				if (adding)
				{
					if (!params[2].empty())
					{
						channel->set_mode_k(params[2]);
						mode_changes += "+k";
					}
					else
					{
						err_needmoreparams(server_name, client, "MODE");
						return;
					}
				}
				else
				{
					channel->set_mode_k("");
					mode_changes += "-k";
				}
				break;
			case 'l':
				if (adding)
				{
					if (params[2].empty())
					{
						err_needmoreparams(server_name, client, "MODE");
						return;
					}
					else
					{
						int limit = atoi(params[2].c_str());
						if (limit > 0)
							channel->set_mode_l(limit);
						else
							channel->set_mode_l(0);
						mode_changes += "+l";
					}
				}
				else
				{
					channel->set_mode_l(0);
					mode_changes += "-l";
				}
				break;
			case 'o':
				{
				if (params[2].empty())
				{
					err_needmoreparams(server_name, client, "MODE");
					return;
				}
				if (server.get_client(params[2]) == NULL)
				{
					err_nosuchnick(server_name, client, params[2], "MODE");
					return;
				}
				if (channel->check_user(params[2]) == NULL)
				{
					err_usernotinchannel(server_name, client, channel, params[2]);
					return;
				}
				Client* nickname = server.get_client(params[2]);
				if (adding)
				{
					channel->add_operator(*nickname);
					std::string announce = ":" + client.get_nickname() + " MODE " + 
											channel->get_name() + " +o " + params[2] + "\r\n";
					const std::vector<Client> &users = channel->get_users();
					for (size_t i = 0; i < users.size(); ++i)
						send(users[i].get_FD(), announce.c_str(), announce.length(), 0);
				}
				else
				{
					channel->rem_operator(*nickname);
					std::string announce = ":" + client.get_nickname() + " MODE " + 
											channel->get_name() + " -o " + params[2] + "\r\n";
					const std::vector<Client> &users = channel->get_users();
					for (size_t i = 0; i < users.size(); ++i)
						send(users[i].get_FD(), announce.c_str(), announce.length(), 0);
				}
				break;
				}
			default:
				if (mode != ' ' && mode != '\t' && mode != '\n' && mode != '\r')
				{
					err_unknownmode(server_name, client, mode);
				}
				break;
		}
	}
	if (!mode_changes.empty())
	{
		std::string announce = ":" + client.get_nickname() + " MODE " + channel->get_name() + " " + mode_changes;
		for (size_t i = 0; i < mode_params.size(); ++i)
			announce += " " + mode_params[i];
		announce += "\r\n";
		const std::vector<Client> &users = channel->get_users();
		for (size_t i = 0; i < users.size(); ++i)
			send(users[i].get_FD(), announce.c_str(), announce.length(), 0);
	}
}