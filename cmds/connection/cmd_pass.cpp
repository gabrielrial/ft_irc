#include "../../lib_irc.hpp"

void cmd_pass(Server &server, RawTextLine &line, Client &client)
{
	//	if (client.is_registered() || client.has_pass())
	//	{
	//		std::string msg = ":localhost 462 * :You may not reregister\r\n";
	//		send(client.get_FD(), msg.c_str(), msg.size(), 0);
	//		return ;
	//	}
	if (line.get_sep_params().empty())
	{
		const std::string msg = ":localhost 461 * PASS :Not enough parameters\r\n";
		send(client.get_FD(), msg.c_str(), msg.size(), 0);
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
	{
		const std::string err = ":localhost 464 * :Password incorrect\r\n";
		send(client.get_FD(), err.c_str(), err.size(), 0);
	}

	if (client.is_registered())
		server.welcome(client);
}
