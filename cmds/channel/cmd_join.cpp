#include "../../lib_irc.hpp"

void	cmd_join(Server &server, RawTextLine &line, Client &client);
int		checkJoinParams(RawTextLine &line, Client &client);
void	broadcast_listupdate(const Channel *chan, const Client &client);

void	cmd_join(Server &server, RawTextLine &line, Client &client)
{
	if (checkJoinParams(line, client) == 1)
		return ;
	// std::string channel_name = line.get_params()[0];
	// if (channel_name[0] != '#' && channel_name[0] != '&' && channel_name[0] != '+' && channel_name[0] != '!')
	// 	channel_name = "#" + channel_name;
	// server.add_channel(channel_name);
	// Channel* channel = server.get_channel(channel_name);
	// if (channel)
	// {
	// 	channel->addUser(client);
	// 	std::string joinMsg = ":" + client.get_nickname() + " JOIN " + channel_name + "\r\n";
	// 	send(client.getFd(), joinMsg.c_str(), joinMsg.length(), 0);
	// 	broadcast_listupdate(channel, client);
	// }
	const std::vector<std::string>& channels = line.get_sep_params();
	for (size_t i = 0; i < channels.size(); ++i)
	{
		std::string channel_name = channels[i];
		if (channel_name[0] != '#' && channel_name[0] != '&' && 
			channel_name[0] != '+' && channel_name[0] != '!')
			channel_name = "#" + channel_name;
		server.add_channel(channel_name);
		Channel* channel = server.get_channel(channel_name);
		if (channel)
		{
			channel->addUser(client);
			std::string joinMsg = ":" + client.get_nickname() + 
								" JOIN " + channel_name + "\r\n";
			send(client.getFd(), joinMsg.c_str(), joinMsg.length(), 0);
			broadcast_listupdate(channel, client);
		}
	}
}

int	checkJoinParams(RawTextLine &line, Client &client)
{
	if (line.get_params().empty())
	{
		std::string error = ":localhost 461 " + client.get_nickname() + " JOIN :Not enough parameters\r\n"; //ERR_NEEDMOREPARAMS
		send(client.getFd(), error.c_str(), error.length(), 0);
		return 1;
	}
	return 0;
}

void	broadcast_listupdate(const Channel *chan, const Client &client)
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
	char serverName[256];
	if (gethostname(serverName, sizeof(serverName)) != 0)
		strcpy(serverName, "localhost");
	std::string names_reply = ":" + std::string(serverName) + " 353 " + 
								client.get_nickname() + " = " + chan->getName() + 
								" :" + user_list + "\r\n"; //RPL_NAMREPLY
	std::string end_names = ":" + std::string(serverName) + " 366 " + 
							client.get_nickname() + " " + chan->getName() + 
							" :End of /NAMES list.\r\n"; //RPL_ENDOFNAMES
	for (size_t i = 0; i < users.size(); ++i)
	{
		const Client &target = users[i];
		send(target.getFd(), names_reply.c_str(), names_reply.length(), 0);
		send(target.getFd(), end_names.c_str(), end_names.length(), 0);
	}
}
