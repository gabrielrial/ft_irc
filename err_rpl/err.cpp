
#include "err_rpl.hpp"

void	err_nosuchnick(std::string server_name, Client &client, std::string nickname, std::string cmd)
{
	std::string err_nosuchnick = ":" + server_name + " 401 " +
								client.get_nickname() + " " + nickname + " " + cmd + " :No such nick\r\n";
	send(client.get_FD(), err_nosuchnick.c_str(), err_nosuchnick.length(), 0);
}

void	err_nosuchchannel(std::string server_name, Client &client, std::string channel_name)
{
	std::string err_nosuchchannel = ":" + server_name + " 403 " +
									client.get_nickname() + " " + channel_name + " :No such channel\r\n";
	send(client.get_FD(), err_nosuchchannel.c_str(), err_nosuchchannel.size(), 0);
}

void	err_norecipient(std::string server_name, Client &client, std::string cmd)
{
	std::string err_norecipient = ":" + server_name + " 411 " +
										client.get_nickname() + " :No recipient given (" + cmd + ")\r\n";
	send(client.get_FD(), err_norecipient.c_str(), err_norecipient.length(), 0);
}

void	err_notexttosend(std::string server_name, Client &client)
{
	std::string err_notexttosend = ":" + server_name + " 412 " +
										client.get_nickname() + " :No text to send\r\n";
	send(client.get_FD(), err_notexttosend.c_str(), err_notexttosend.length(), 0);
}

void	err_nonicknamegiven(std::string server_name, Client &client)
{
	std::string err_nonicknamegiven = ":" + server_name + " 431 " +
						client.get_nickname() + " :No nickname given\r\n";
	send(client.get_FD(), err_nonicknamegiven.c_str(), err_nonicknamegiven.size(), 0);
}

void	err_erroneusnickname(std::string server_name, Client &client, std::string newNick)
{
	std::string err_erroneusnickname = ":" + server_name + " 432 " +
						client.get_nickname() + " " + newNick + " :Erroneus nickname\r\n";
	send(client.get_FD(), err_erroneusnickname.c_str(), err_erroneusnickname.size(), 0);
}

void	err_nicknameinuse(std::string server_name, Client &client, std::string attempt)
{
	std::string err_nicknameinuse = ":" + server_name + " 433 " +
						client.get_nickname() + " " + attempt + " :Nickname is already in use\r\n";
	send(client.get_FD(), err_nicknameinuse.c_str(), err_nicknameinuse.size(), 0);
}

void	err_usernotinchannel(std::string server_name, Client &client, Channel *channel, std::string nickname)
{
	std::string err_usernotinchannel = ":" + server_name + " 441 " +
									client.get_nickname() + " " + nickname + " " + channel->get_name() + " :They aren't on that channel\r\n";
	send(client.get_FD(), err_usernotinchannel.c_str(), err_usernotinchannel.length(), 0);
}

void	err_notonchannel(std::string server_name, Client &client, Channel *channel)
{
	std::string err_notonchannel = ":" + server_name + " 442 " +
									client.get_nickname() + " " + channel->get_name() + " :You're not on that channel\r\n";
	send(client.get_FD(), err_notonchannel.c_str(), err_notonchannel.size(), 0);
}

void	err_useronchannel(std::string server_name, Client &client, std::string target_cli, std::string channel_name)
{
	std::string err_useronchannel = ":" + server_name + " 443 " +
										client.get_nickname() + " " + target_cli + " " + channel_name +" :is already on channel\r\n";
	send(client.get_FD(), err_useronchannel.c_str(), err_useronchannel.length(), 0);
}

void	err_needmoreparams(std::string server_name, Client &client, std::string cmd)
{
	std::string err_needmoreparams = ":" + server_name + " 461 " +
									client.get_nickname() + " " + cmd + " :Not enough parameters\r\n";
	send(client.get_FD(), err_needmoreparams.c_str(), err_needmoreparams.length(), 0);
}

void	err_alreadyregistered(std::string server_name, Client &client)
{
	std::string err_alreadyregistered = ":" + server_name + " 462 * :You may not reregister\r\n";
	send(client.get_FD(), err_alreadyregistered.c_str(), err_alreadyregistered.size(), 0);
}

void	err_passwdmismatch(std::string server_name, Client &client)
{
	std::string err_passwdmismatch = ":" + server_name + " 464 * :Password incorrect\r\n";
	send(client.get_FD(), err_passwdmismatch.c_str(), err_passwdmismatch.size(), 0);
}

void	err_channelisfull(std::string server_name, Client &client, Channel *channel)
{
	std::string err_channelisfull = ":" + server_name + " 471 " +
									client.get_nickname() + " " + channel->get_name() + " :Cannot join channel (+l)\r\n";
	send(client.get_FD(), err_channelisfull.c_str(), err_channelisfull.length(), 0);
}

void	err_unknownmode(std::string server_name, Client &client, char mode)
{
	std::string err_unknownmode = ":" + server_name + " 472 " +
								client.get_nickname() + " " + mode + " :is unknown mode char to me\r\n";
	send(client.get_FD(), err_unknownmode.c_str(), err_unknownmode.length(), 0);
}

void	err_badchannelkey(std::string server_name, Client &client, Channel *channel)
{
	std::string err_badchannelkey = ":" + server_name + " 475 " +
									client.get_nickname() + " " + channel->get_name() + " :Cannot join channel (+k)\r\n";
	send(client.get_FD(), err_badchannelkey.c_str(), err_badchannelkey.length(), 0);
}

void	err_badchanmask(std::string server_name, Client &client, std::string channel_name)
{
	std::string err_badchanmask = ":" + server_name + " 476 " +
								client.get_nickname() + " " + channel_name + " :Bad Channel Mask\r\n";
	send(client.get_FD(), err_badchanmask.c_str(), err_badchanmask.length(), 0);
}

void	err_chanoprivsneeded(std::string server_name, Client &client, Channel *channel)
{
	std::string err_chanoprivsneeded = ":" + server_name + " 482 " +
										client.get_nickname() + " " + channel->get_name() + " :You're not channel operator\r\n";
	send(client.get_FD(), err_chanoprivsneeded.c_str(), err_chanoprivsneeded.length(), 0);
}