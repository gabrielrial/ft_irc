
#include "err_rpl.hpp"

std::string	get_servername()
{
	char server_name[256];
	if (gethostname(server_name, sizeof(server_name)) != 0)
		strcpy(server_name, "localhost");
	return server_name;
}

void	err_needmoreparams(std::string server_name, Client &client, std::string cmd)
{
	std::string err_needmoreparams = ":" + server_name + " 461 " + 
									client.get_nickname() + " " + cmd + " :Not enough parameters\r\n";
	send(client.get_FD(), err_needmoreparams.c_str(), err_needmoreparams.length(), 0);
}

void	err_nosuchchannel(std::string server_name, Client &client, std::string channel_name)
{
	std::string err_nosuchchannel = ":" + server_name + " 403 " + 
									client.get_nickname() + " " + channel_name + " :No such channel\r\n";
	send(client.get_FD(), err_nosuchchannel.c_str(), err_nosuchchannel.size(), 0);
}

void	err_notonchannel(std::string server_name, Client &client, Channel *channel)
{
	std::string err_notonchannel = ":" + server_name + " 442 " + 
									client.get_nickname() + " " + channel->get_name() + " :You're not on that channel\r\n";
	send(client.get_FD(), err_notonchannel.c_str(), err_notonchannel.size(), 0);
}

void	err_chanoprivsneeded(std::string server_name, Client &client, Channel *channel)
{
	std::string err_chanoprivsneeded = ":" + server_name + " 482 " + 
									client.get_nickname() + " " + channel->get_name() + " :You're not channel operator\r\n";
	send(client.get_FD(), err_chanoprivsneeded.c_str(), err_chanoprivsneeded.length(), 0);
}

void	err_nosuchnick(std::string server_name, Client &client, std::string nickname, std::string cmd)
{
	std::string err_nosuchnick = ":" + server_name + " 401 " + 
								client.get_nickname() + " " + nickname + " " + cmd + " :No such nick\r\n";
	send(client.get_FD(), err_nosuchnick.c_str(), err_nosuchnick.length(), 0);
}

void	err_unknownmode(std::string server_name, Client &client, char mode)
{
	std::string err_unknownmode = ":" + server_name + " 472 " + 
								client.get_nickname() + " " + mode + " :is unknown mode char to me\r\n";
	send(client.get_FD(), err_unknownmode.c_str(), err_unknownmode.length(), 0);
}

void	err_usernotinchannel(std::string server_name, Client &client, Channel *channel, std::string nickname)
{
	std::string err_usernotinchannel = ":" + server_name + " 441 " + 
								client.get_nickname() + " " + nickname + " " + channel->get_name() + " :They aren't on that channel\r\n";
	send(client.get_FD(), err_usernotinchannel.c_str(), err_usernotinchannel.length(), 0);
}

void	err_badchannelkey(std::string server_name, Client &client, Channel *channel)
{
	std::string err_badchannelkey = ":" + server_name + " 475 " + 
								client.get_nickname() + " " + channel->get_name() + " :Cannot join channel (+k)\r\n";
	send(client.get_FD(), err_badchannelkey.c_str(), err_badchannelkey.length(), 0);
}

void	err_channelisfull(std::string server_name, Client &client, Channel *channel)
{
	std::string err_channelisfull = ":" + server_name + " 471 " + 
								client.get_nickname() + " " + channel->get_name() + " :Cannot join channel (+l)\r\n";
	send(client.get_FD(), err_channelisfull.c_str(), err_channelisfull.length(), 0);
}