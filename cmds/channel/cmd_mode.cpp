#include "../../lib_irc.hpp"

void cmd_mode(Server &server, RawTextLine &line, Client &client);
int check_mode_params(RawTextLine &line, Client &client, std::string server_name);
int check_mode_channel(Client &client, Channel *channel, std::string server_name);

void change_mode(Server &server, Client &client, Channel *channel, 
				const std::vector<std::string> &params, std::string server_name);

void cmd_mode(Server &server, RawTextLine &line, Client &client)
{
	char server_name[256];
	if (gethostname(server_name, sizeof(server_name)) != 0)
		strcpy(server_name, "localhost");
	if (check_mode_params(line, client, server_name) == 1)
		return;
	const std::vector<std::string> &params = line.get_params();
	std::string channel_name = params[0];
	Channel *channel = server.get_channel(channel_name);
	if (check_mode_channel(client, channel, server_name) == 1)
		return;
	if (params.size() == 1)
	{
		std::string modestring = channel->get_allmode();
		std::string rpl_channelmodeis = ":" + std::string(server_name) + " 324 " + 
							client.get_nickname() + " " + channel_name + " " + 
							modestring + "\r\n";
		send(client.get_FD(), rpl_channelmodeis.c_str(), rpl_channelmodeis.length(), 0);
		return;
	}
	change_mode(server, client, channel, params, server_name);
}

int check_mode_params(RawTextLine &line, Client &client, std::string server_name)
{
	if (line.get_params().empty())
	{
		std::string err_needmoreparams = ":" + std::string(server_name) + " 461 " + 
				client.get_nickname() + " MODE :Not enough parameters\r\n";
		send(client.get_FD(), err_needmoreparams.c_str(), err_needmoreparams.length(), 0);
		return 1;
	}
	return 0;
}

int check_mode_channel(Client &client, Channel *channel, std::string server_name)
{
	if (!channel)
	{
		std::string err_nosuchchannel = ":" + std::string(server_name) + " 403 " + 
				client.get_nickname() + " " + channel->get_name() + " :No such channel\r\n";
		send(client.get_FD(), err_nosuchchannel.c_str(), err_nosuchchannel.size(), 0);
		return 1;
	}
	else if (!channel->has_user(client))
	{
		std::string err_notonchannel = ":" + std::string(server_name) + " 442 " + 
				client.get_nickname() + " " + channel->get_name() + " :You're not on that channel\r\n";
		send(client.get_FD(), err_notonchannel.c_str(), err_notonchannel.size(), 0);
		return 1;
	}
	else if (!channel->is_operator(client))
	{
		std::string err_chanoprivsneeded = ":" + server_name + " 482 " + 
							client.get_nickname() + " " + channel->get_name() + 
							" :You're not channel operator\r\n";
		send(client.get_FD(), err_chanoprivsneeded.c_str(), err_chanoprivsneeded.length(), 0);
		return 1;
	}
	return 0;
}
//GPT generated, rethink!!!!!
//handle unknown mode error (472 ERR_UNKNOWNMODE)
void change_mode(Server &server, Client &client, Channel *channel,
				const std::vector<std::string> &params, std::string server_name)
{
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
				break;
			case 't':
				channel->set_mode_t(adding);
				break;
			case 'k':
				if (adding)
				{
					if (!params[2].empty())
						channel->set_mode_k(params[2]);
					else
					{
						std::string err_needmoreparams = ":" + std::string(server_name) + " 461 " + 
														client.get_nickname() + " MODE :Not enough parameters\r\n";
						send(client.get_FD(), err_needmoreparams.c_str(), err_needmoreparams.length(), 0);
						return;
					}
				}
				else
					channel->set_mode_k("");
				break;
			case 'l':
				if (adding)
				{
					if (params[2].empty())
					{
						std::string err_needmoreparams = ":" + std::string(server_name) + " 461 " + 
														client.get_nickname() + " MODE :Not enough parameters\r\n";
						send(client.get_FD(), err_needmoreparams.c_str(), err_needmoreparams.length(), 0);
						return;
					}
					else
					{
						//int limit = std::stoi(params[2]);
						int limit = atoi(params[2].c_str());
						if (limit > 0)
							channel->set_mode_l(limit);
						else
							channel->set_mode_l(0);
					}
				}
				else
					channel->set_mode_l(0);
				break;
			case 'o':
				{
				if (params[2].empty())
				{
					std::string err_needmoreparams = ":" + std::string(server_name) + " 461 " + 
													client.get_nickname() + " MODE :Not enough parameters\r\n";
					send(client.get_FD(), err_needmoreparams.c_str(), err_needmoreparams.length(), 0);
					return;
				}
				if (server.get_client(params[2]) == NULL)
				{
					std::string err_nosuchnick = ":" + std::string(server_name) + " 401 " + 
												client.get_nickname() + " " + params[2] + " MODE :No such nick\r\n";
					send(client.get_FD(), err_nosuchnick.c_str(), err_nosuchnick.length(), 0);
					return;
				}
				Client* nickname = server.get_client(params[2]);
				if (adding)
				{
					channel->add_operator(*nickname);
					std::string announce = ":" + client.get_prefix() + " MODE " + 
											channel->get_name() + " +o " + params[2] + "\r\n";
					const std::vector<Client>& users = channel->get_users();
					for (size_t i = 0; i < users.size(); ++i)
						send(users[i].get_FD(), announce.c_str(), announce.length(), 0);
				}
				else
				{
					channel->rem_operator(*nickname);
					std::string announce = ":" + client.get_prefix() + " MODE " + 
											channel->get_name() + " -o " + params[2] + "\r\n";
					const std::vector<Client>& users = channel->get_users();
					for (size_t i = 0; i < users.size(); ++i)
						send(users[i].get_FD(), announce.c_str(), announce.length(), 0);
				}
				break;
				}
			default:
				if (mode != ' ' && mode != '\t' && mode != '\n' && mode != '\r')
				{
					std::string err_unknownmode = ":" + server_name + " 472 " + 
												client.get_nickname() + " " + mode + " :is unknown mode char to me\r\n";
					send(client.get_FD(), err_unknownmode.c_str(), err_unknownmode.length(), 0);
				}
				break;
		}
	}
}