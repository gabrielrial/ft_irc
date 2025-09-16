#include "../../lib_irc.hpp"

void cmd_privmsg(Server &server, RawTextLine &line, Client &client)
{
	std::vector<Client *> client_list;
	server.check_client(line, client_list);

	if (client_list.empty())
		return;

	for (size_t i = 0; i < client_list.size(); ++i)
	{
		Client *target = client_list[i];
		std::string msg = std::string(":") + client.get_nickname() + " PRIVMSG " + target->get_username() + " :" + line.get_trailing() + "\r\n";
		std::cout << msg << std::endl;

		send(target->get_FD(), msg.c_str(), msg.size(), 0);
	}
}