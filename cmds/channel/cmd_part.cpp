#include "../../lib_irc.hpp"

void	cmd_part(Server &server, RawTextLine &line, Client &client);
void	broadcast_part(const Channel *chan, const Client &client, const std::string &reason);

void	cmd_part(Server &server, RawTextLine &line, Client &client)
{
	std::string server_name = server.get_servername();
	if (line.get_params().empty())
	{
		err_needmoreparams(server_name, client, "PART");
		return;
	}
	const std::vector<std::string> &params = line.get_params();
	std::string reason = "";
	if (!line.get_trailing().empty())
		reason = line.get_trailing();
	std::string channels = params[0];
	std::istringstream channel_stream(channels); //splitting
	std::string channel_name;
	while (std::getline(channel_stream, channel_name, ','))
	{
		Channel *channel = server.get_channel(channel_name);
		if (!channel)
			err_nosuchchannel(server_name, client, channel_name);
		else if (!channel->has_user(&client))
			err_notonchannel(server_name, client, channel);
		else
		{
			broadcast_part(channel, client, reason);
			channel->remove_user(&client);
		}
	}
}

void broadcast_part(const Channel *chan, const Client &client, const std::string &reason)
{
	if (!chan)
		return;
	std::string part_msg = client.get_prefix() + " PART " + 
						chan->get_name();
	if (!reason.empty())
		part_msg += " :" + reason;
	part_msg += "\r\n";
	//std::cout << "broadcasted: " << part_msg << std::endl;
	const std::vector<Client*>& users = chan->get_users();
	for (size_t i = 0; i < users.size(); ++i)
		send((*users[i]).get_FD(), part_msg.c_str(), part_msg.length(), 0);
}
