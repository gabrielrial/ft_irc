#include "../../lib_irc.hpp"

void cmd_pass(Server &server, RawTextLine &line, Client &client)
{
	if (client.is_registered() || client.has_pass())
		return ;
	if (line.get_sep_params().empty())
	{
		err_needmoreparams(server.get_servername(), client, "PASS");
		return;
	}
	std::string provided = line.get_sep_params()[0];
	if (!provided.empty() && provided[0] == ':') // some clients enter ':' before the actual pw
		provided.erase(0, 1);
	if (provided == server.get_password())
	{
		client.pass_accepted(true);
		std::string ok = ":localhost NOTICE * :Password accepted\r\n";
		send(client.get_FD(), ok.c_str(), ok.size(), 0);
	}
	else
		err_passwdmismatch(server.get_servername(), client);
	if (client.is_registered())
		server.welcome(client);
}
