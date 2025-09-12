#include "../../lib_irc.hpp"

int	checkJoinParams(RawTextLine &line, Client &client);

void	cmd_join(Server &server, RawTextLine &line, Client &client)
{
	if (checkJoinParams(line, client) == 1)
		return ;
	std::string channel_name = line.getParams()[0];
	if (channel_name[0] != '#')
		channel_name = "#" + channel_name;
	server.addChannel(channel_name);
	Channel* channel = server.getChannel(channel_name);
	if (channel)
	{
		channel->addUser(client);
		std::string joinMsg = ":" + client.getNickname() + " JOIN " + channel_name + "\r\n";
		send(client.getFd(), joinMsg.c_str(), joinMsg.length(), 0);
	}
		return;
}

int	checkJoinParams(RawTextLine &line, Client &client)
{
	if (line.getParams().empty())
	{
		std::string error = ":localhost 461 " + client.getNickname() + " JOIN :Not enough parameters\r\n";
		send(client.getFd(), error.c_str(), error.length(), 0);
		return 1;
	}
	return 0;
}