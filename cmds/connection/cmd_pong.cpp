#include "../../lib_irc.hpp"

void cmd_pong(Server &server, RawTextLine &line, Client &client)
{
	if (line.get_trailing() == server.ping)
	{
		std::cout << "cmd_pong()" << std::endl;
		if (!client.get_pong())
			client.set_pong();
		return;
	}
}