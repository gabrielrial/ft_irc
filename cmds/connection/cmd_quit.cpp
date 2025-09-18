#include "../../lib_irc.hpp"

void cmd_quit(Server &server, RawTextLine &line, Client &client)
{
	const std::string msg = client.get_prefix() + "QUIT" + CRLF;
	const std::string quit_msg = line.get_trailing().empty() ? msg : line.get_trailing();
	std::vector<Channel> channels = server.get_vector_channels();
	for (size_t i = 0; i < channels.size(); i++) // broadcast quit msg to all channels
	{
		const std::string &name = channels[i].get_name();
		Channel *chan = server.get_channel(name);  // modifiable
		if (!chan)
			continue;

		if (!chan->has_user(client))
			continue; // check
		const std::vector<Client> &users = chan->get_users();
		for (size_t j = 0; j < users.size(); j++)
		{
			if (users[j].get_FD() == client.get_FD())
				continue;
			else
				send(users[j].get_FD(), quit_msg.c_str(), quit_msg.size(), 0); // broadcast quit_msg to other chan members
		}
		chan->remove_user(client); // remove client from channels
	}
	server.schedule_close(client.get_FD());
}
