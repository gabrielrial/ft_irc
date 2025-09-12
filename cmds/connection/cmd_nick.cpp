#include "../../lib_irc.hpp"

void cmd_nick(Server &server, RawTextLine &line, Client &client)
{
	(void)server;
	(void)line;
	client.set_nickname(line.get_sep_params()[0]);
}
