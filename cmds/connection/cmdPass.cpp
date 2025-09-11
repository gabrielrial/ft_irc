#include "../../lib_irc.hpp"

void cmdPass(Server &server, RawTextLine &line, Client &client)
{
	(void)server;
	(void)line;
    if (line.getSepParams().empty())
        return;
    if (line.getSepParams()[0] == PASSWORD)
		return;
	else
	{
		close(client.getFd());
		std::cout << "### DELETE USER FROM CLIENTS AFTER WRONG PASSWORD ###" << std::endl;
	}
}