#include "../../lib_irc.hpp"

void cmd_nick(Server &server, RawTextLine &line, Client &client)
{
	if (line.get_sep_params().empty())
	{
		const std::string msg = ":localhost 431 * :No nickname given\r\n";
		send(client.get_FD(), msg.c_str(), msg.size(), 0);
		return;
	}
	const std::string newNick = line.get_sep_params()[0];
	if (newNick.size() > 30 || newNick.find_first_of(" ,*?!@.:#&") != std::string::npos)
	{
		const std::string msg = ":localhost 432 " + client.get_nickname() + " " + newNick + " :Erroneous nickname\r\n";
		send(client.get_FD(), msg.c_str(), msg.size(), 0);
		return;
	}
	if (client.get_nickname() == newNick) return ;
	if (server.check_nick_uniqueness(newNick) == false)
	{
		const std::string attempt = line.get_sep_params()[0];
		const std::string msg = ":localhost 433 " + client.get_nickname() + " " + attempt + " :Nickname is already in use\r\n";
		send(client.get_FD(), msg.c_str(), msg.size(), 0);
		return ;
	}
	if (client.has_nick()) // if 'correctly'/'successfully' changing nickname
	{
		const std::string oldNick = client.get_nickname();
		client.set_nickname(newNick);
		std::string nickMsg = ":" + oldNick + "!" + client.get_username() + "@" + client.get_hostname()
			+ " NICK :" + newNick + "\r\n";
		const std::string msg = "You are now known as " + line.get_sep_params()[0] + "\r\n";
		send(client.get_FD(), msg.c_str(), msg.size(), 0);
	}
	client.set_nickname(line.get_sep_params()[0]);
}
