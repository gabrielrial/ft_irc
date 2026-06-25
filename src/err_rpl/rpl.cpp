# include "../lib_irc.hpp"

void	rpl_notopic(std::string server_name, Client &client, std::string channel_name)
{
	std::string rpl_notopic = ":" + std::string(server_name) + " 331 " + 
					client.get_nickname() + " " + channel_name + " :No topic is set\r\n";
	send(client.get_FD(), rpl_notopic.c_str(), rpl_notopic.size(), 0);
}

void	rpl_topic(std::string server_name, Client &client, std::string channel_name, std::string topic)
{
	std::string rpl_topic = ":" + std::string(server_name) + " 332 " + 
					client.get_nickname() + " " + channel_name + " :" + topic + "\r\n";
	send(client.get_FD(), rpl_topic.c_str(), rpl_topic.size(), 0);
}

void	rpl_namreply(const std::string server_name, const Client &client, const std::string channel_name, const std::string user_list)
{
	std::string rpl_namreply = ":" + server_name + " 353 " + 
							client.get_nickname() + " = " + channel_name + " :" + user_list + "\r\n";
	send(client.get_FD(), rpl_namreply.c_str(), rpl_namreply.length(), 0);
}

void	rpl_endofnames(const std::string server_name, const Client &client, const std::string channel_name)
{
	std::string rpl_endofnames = ":" + server_name + " 366 " + 
								client.get_nickname() + " " + channel_name + " :End of /NAMES list.\r\n";
	send(client.get_FD(), rpl_endofnames.c_str(), rpl_endofnames.length(), 0);
}

void	rpl_channelmodeis(std::string server_name, const Client &client, std::string channel_name, std::string modestring)
{
	std::string rpl_channelmodeis = ":" + server_name + " 324 " + 
								client.get_nickname() + " " + channel_name + " " + modestring + "\r\n";
	send(client.get_FD(), rpl_channelmodeis.c_str(), rpl_channelmodeis.length(), 0);
}