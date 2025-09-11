#include "../../lib_irc.hpp"

void cmdNick(Server &server, RawTextLine &line, Client &client)
{
	(void)server;
	(void)line;
	client.setNickname(line.getSepParams()[0]);
}
