#include "../../lib_irc.hpp"

#include "../../lib_irc.hpp"

void cmd_part(Server &server, RawTextLine &line, Client &client);
int check_part_params(RawTextLine &line, Client &client, char *server_name);
void broadcast_part(const Channel *chan, const Client &client, const std::string &reason);

void cmd_part(Server &server, RawTextLine &line, Client &client)
{
	char server_name[256];
	if (gethostname(server_name, sizeof(server_name)) != 0)
		strcpy(server_name, "localhost");
	if (check_part_params(line, client, server_name) == 1)
		return;
	const std::vector<std::string>& params = line.get_params();
	std::string first_param = params[0];
	std::string reason = (params.size() > 1) ? params[1] : "";
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
		Channel* channel = server.get_channel(channel_name);
		if (!channel)
		{
			std::string error = ":" + std::string(server_name) + " 403 " + 
							   client.get_nickname() + " " + channel_name + 
							   " :No such channel\r\n";
			send(client.getFd(), error.c_str(), error.length(), 0);
		}
		else if (!channel->hasUser(client))
		{
			std::string error = ":" + std::string(server_name) + " 442 " + 
							   client.get_nickname() + " " + channel_name + 
							   " :You're not on that channel\r\n";
			send(client.getFd(), error.c_str(), error.length(), 0);
		}
		else
		{
			broadcast_part(channel, client, reason);
			channel->removeUser(client);
		}
		start = end + 1;
	}
}

int check_part_params(RawTextLine &line, Client &client, char *server_name)
{
	if (line.get_params().empty())
	{
		std::string error = ":" + std::string(server_name) + " 461 " + 
						   client.get_nickname() + " PART " +
						   ":Not enough parameters\r\n";
		send(client.getFd(), error.c_str(), error.length(), 0);
		return 1;
	}
	return 0;
}

void broadcast_part(const Channel *chan, const Client &client, const std::string &reason)
{
	if (!chan)
		return;
		
	std::string partMsg = ":" + client.get_nickname() + " PART " + 
						 chan->getName();
	if (!reason.empty())
		partMsg += " :" + reason;
	partMsg += "\r\n";
	
	// Send to all users in channel including the leaving user
	const std::vector<Client>& users = chan->getUsers();
	for (size_t i = 0; i < users.size(); ++i)
	{
		send(users[i].getFd(), partMsg.c_str(), partMsg.length(), 0);
	}
}