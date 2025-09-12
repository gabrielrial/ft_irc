#include "../../lib_irc.hpp"

void cmd_nick(Server &server, RawTextLine &line, Client &client)
{
	(void)server;
	(void)line;
	client.setNickname(line.getSepParams()[0]);
}
