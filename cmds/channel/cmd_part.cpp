#include "../../lib_irc.hpp"

void	cmd_part(Server &server, RawTextLine &line, Client &client);
int		check_part_params(RawTextLine &line, Client &client, char *server_name);
//void	broadcast_part(const Channel *chan, const Client &client, const std::string &reason);
void	broadcast_part(const Channel *chan, const Client &client);

//#define RES "\033[0m"
//#define RED "\033[31m"

void	cmd_part(Server &server, RawTextLine &line, Client &client)
{
	char server_name[256];
	if (gethostname(server_name, sizeof(server_name)) != 0)
		strcpy(server_name, "localhost");
	if (check_part_params(line, client, server_name) == 1)
		return;
	const std::vector<std::string> &params = line.get_params();
	std::string first_param = params[0];
	//std::string reason = (params.size() > 1) ? params[1] : "";
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
			channel_name = "#" + channel_name;
		Channel *channel = server.get_channel(channel_name);
		//std::cout << RED << "Channel:" << channel << RES << std::endl;
		//std::cout << RED << "Channel name:" << channel_name << RES << std::endl;
		if (!channel)
		{
			std::string err_nosuchchannel = ":" + std::string(server_name) + " 403 " + 
							client.get_nickname() + " " + channel_name + 
							" :No such channel\r\n";
			send(client.get_FD(), err_nosuchchannel.c_str(), err_nosuchchannel.length(), 0);
		}
		else if (!channel->has_user(client))
		{
			std::string err_notonchannel = ":" + std::string(server_name) + " 442 " + 
							client.get_nickname() + " " + channel_name + 
							" :You're not on that channel\r\n";
			send(client.get_FD(), err_notonchannel.c_str(), err_notonchannel.length(), 0);
		}
		else
		{
			//broadcast_part(channel, client, reason);
			broadcast_part(channel, client);
			channel->remove_user(client);
		}
		start = end + 1;
	}
}

int	check_part_params(RawTextLine &line, Client &client, char *server_name)
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

//void broadcast_part(const Channel *chan, const Client &client, const std::string &reason)
void	broadcast_part(const Channel *chan, const Client &client)
{
	if (!chan)
		return;
	std::string part_msg = ":" + client.get_nickname() + " PART " + 
						 chan->get_name() + "\r\n";
	//if (!reason.empty())
	//	part_msg += " :" + reason;
	//part_msg += "\r\n";
	const std::vector<Client>& users = chan->get_users();
	for (size_t i = 0; i < users.size(); ++i)
		send(users[i].get_FD(), part_msg.c_str(), part_msg.length(), 0);
}
