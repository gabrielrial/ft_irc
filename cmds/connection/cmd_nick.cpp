#include "../../lib_irc.hpp"

void cmd_nick(Server &server, RawTextLine &line, Client &client)
{
	(void)server;
	if (client.has_nick())
	{
		const std::string msg = "You are now known as " + line.get_sep_params()[0] + "\r\n";
		send(client.get_FD(), msg.c_str(), msg.size(), 0);
	}
	client.set_nickname(line.get_sep_params()[0]);
}
