#include "../../lib_irc.hpp"

void cmd_nick(Server &server, RawTextLine &line, Client &client)
{
	if (line.get_sep_params().empty())
	{
		err_nonicknamegiven(server.get_servername(), client);
		return;
	}
	const std::string newNick = line.get_sep_params()[0];
	if (client.get_nickname() == newNick)
		return;
	if (newNick.size() > 30 || newNick.find_first_of(" ,*?!@.:#&") != std::string::npos)
	{
		err_erroneusnickname(server.get_servername(), client, newNick);
		return;
	}
	if (server.check_nick_uniqueness(newNick) == false)
	{
		const std::string attempt = line.get_sep_params()[0];
		err_nicknameinuse(server.get_servername(), client, attempt);
		return;
	}
	if (client.has_nick())
	{
		const std::string oldNick = client.get_nickname();
		std::string nickMsg = ":" + oldNick + "!" + client.get_username() + "@" + client.get_hostname() + " NICK :" + newNick + "\r\n";
		const std::string msg = "You are now known as " + line.get_sep_params()[0] + "\r\n";
		send(client.get_FD(), msg.c_str(), msg.size(), 0);
		client.set_nickname(newNick);
		return;
	}
	client.set_nickname(newNick);
	if (client.is_registered())
		server.welcome(client);
}
