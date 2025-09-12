#include "../../lib_irc.hpp"

void cmd_pass(Server &server, RawTextLine &line, Client &client)
{
	(void)server;
	(void)line;
    if (line.getSepParams().empty())
        return;
    if (line.getSepParams()[0] == server.getPassword())
		return;
	else
	{
		close(client.getFd());
		std::cout << "### DELETE USER FROM CLIENTS AFTER WRONG PASSWORD ###" << std::endl;
	}
}