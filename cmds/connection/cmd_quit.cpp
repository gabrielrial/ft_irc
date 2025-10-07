#include "../../lib_irc.hpp"

void	cmd_quit(Server &server, RawTextLine &line, Client &client);
void	broadcast_quit(const Channel *chan, const Client &client, const std::string &reason);

void cmd_quit(Server &server, RawTextLine &line, Client &client)
{
	std::string reason;
	if (line.get_trailing().empty())
		reason = "Client Quit";
	else
		reason = line.get_trailing();
	std::vector<Channel> channels = server.get_vector_channels();
	for (size_t i = 0; i < channels.size(); i++) // broadcast quit msg to all channels
	{
		const std::string &name = channels[i].get_name();
		Channel *chan = server.get_channel(name);  // modifiable
		broadcast_quit(chan, client, reason);
		chan->remove_user(client); // remove client from channels
	}
	server.schedule_close(client.get_FD());
}

void broadcast_quit(const Channel *chan, const Client &client, const std::string &reason)
{
	if (!chan)
		return;
	std::string quit_msg = client.get_prefix() + " QUIT " + 
						chan->get_name();
	if (!reason.empty())
		quit_msg += " :" + reason;
	quit_msg += "\r\n";
	const std::vector<Client>& users = chan->get_users();
	for (size_t i = 0; i < users.size(); ++i)
		send(users[i].get_FD(), quit_msg.c_str(), quit_msg.length(), 0);
}