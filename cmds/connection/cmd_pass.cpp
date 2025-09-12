#include "../../lib_irc.hpp"

void cmd_pass(Server &server, RawTextLine &line, Client &client)
{
	(void)server;
	(void)line;
    if (line.get_sep_params().empty())
        return;
    if (line.get_sep_params()[0] == server.get_password())
		return;
	else
	{
		close(client.getFd());
		std::cout << "### DELETE USER FROM CLIENTS AFTER WRONG PASSWORD ###" << std::endl;
	}
}