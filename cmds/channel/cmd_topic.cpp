#include "../../lib_irc.hpp"

void	cmd_topic(Server &server, RawTextLine &line, Client &client);
int		check_topic_params(RawTextLine &line, Client &client, std::string server_name);
int		check_topic_channel(Server &server, Client &client, std::string channel_name, std::string server_name);
void	topic_setquery();

#define RES "\033[0m"
#define RED "\033[31m"

void	cmd_topic(Server &server, RawTextLine &line, Client &client)
{
	(void)server;
	//(void)line;
	//(void)(client);
	char server_name[256];
	if (gethostname(server_name, sizeof(server_name)) != 0)
		strcpy(server_name, "localhost");
	if (check_topic_params(line, client, server_name) == 1)
		return;
	std::cout << RED << "works fine" << RES << std::endl;
	const std::vector<std::string> &params = line.get_sep_params();
	std::string channel_name = params[0];
	if (check_topic_channel(server, client, channel_name, server_name) == 1);
		return;
	
	// std::string channel_topic = params[1];
	// Channel *channel = server.get_channel(channel_name);
	// std::cout << RED << "Channel name: " << channel_name << RES << std::endl;
	// std::cout << RED << "Channel: " << channel << RES << std::endl;
	//if (params[1])

	// if (params.size() == 1)
	// {
	// 	std::string print_topic = channel->get_topic() + "\r\n";
	// 	send(client.get_FD(), print_topic.c_str(), print_topic.length(), 0);
	// }
	// else
	// 	channel->set_topic(params[1]);
}

int	check_topic_params(RawTextLine &line, Client &client, std::string server_name)
{
	if (line.get_params().empty())
	{
		std::string err_needmoreparams = ":" + std::string(server_name) + " 461 " + 
				client.get_nickname() + " PART :Not enough parameters\r\n";
		send(client.get_FD(), err_needmoreparams.c_str(), err_needmoreparams.length(), 0);
		return 1;
	}
	return 0;
}

int	check_topic_channel(Server &server, Client &client, std::string channel_name, std::string server_name)
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