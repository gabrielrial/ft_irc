#include "../../lib_irc.hpp"

void	cmd_join(Server &server, RawTextLine &line, Client &client);
int		check_join_params(RawTextLine &line, Client &client, char *server_name);
void	broadcast_listupdate(const Channel *chan, const Client &client, char *server_name); //previously names_list() in server.cpp

void	cmd_join(Server &server, RawTextLine &line, Client &client)
{
	char server_name[256];
	if (gethostname(server_name, sizeof(server_name)) != 0)
		strcpy(server_name, "localhost");
	if (check_join_params(line, client, server_name) == 1)
		return ;
	const std::vector<std::string>& params = line.get_params();
	std::string first_param = params[0];
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
		server.add_channel(channel_name);
		Channel* channel = server.get_channel(channel_name);
		channel->addUser(client);
		std::string joinMsg = ":" + client.get_nickname() + 
							" JOIN " + channel_name + "\r\n";
		send(client.getFd(), joinMsg.c_str(), joinMsg.length(), 0);
		broadcast_listupdate(channel, client, server_name);
		start = end + 1;
	}
}

int	check_join_params(RawTextLine &line, Client &client, char *server_name)
{
	if (line.get_params().empty())
	{
		std::string err_needmoreparams = ":" + std::string(server_name) + " 461 " + client.get_nickname() + 
		" JOIN :Not enough parameters\r\n"; //ERR_NEEDMOREPARAMS
		send(client.getFd(), err_needmoreparams.c_str(), error.length(), 0);
		return 1;
	}
	return 0;
}

void	broadcast_listupdate(const Channel *chan, const Client &client, char *server_name)
{
	if (!chan)
		return;
	const std::vector<Client> &users = chan->getUsers();
	std::string user_list;
	for (size_t i = 0; i < users.size(); ++i)
	{
		if (i > 0)
			user_list += " ";
		user_list += users[i].get_nickname();
	}
	std::string rpl_namreply = ":" + std::string(server_name) + " 353 " + 
								client.get_nickname() + " = " + chan->getName() + 
								" :" + user_list + "\r\n"; //RPL_NAMREPLY
	std::string rpl_endofnames = ":" + std::string(server_name) + " 366 " + 
							client.get_nickname() + " " + chan->getName() + 
							" :End of /NAMES list.\r\n"; //RPL_ENDOFNAMES
	for (size_t i = 0; i < users.size(); ++i)
	{
		const Client &target = users[i];
		send(target.getFd(), rpl_namreply.c_str(), rpl_namreply.length(), 0);
		send(target.getFd(), rpl_endofnames.c_str(), rpl_endofnames.length(), 0);
	}
}
