#include "../../lib_irc.hpp"

void cmd_user(Server &server, RawTextLine &line, Client &client)
{
	(void)server;
	if (client.is_registered()) // Already registered → ERR_ALREADYREGISTRED
	{
		const std::string msg = ":localhost 462 * :You may not reregister\r\n";
		send(client.get_FD(), msg.c_str(), msg.size(), 0);
		return;
	}
	const std::vector<std::string> &params = line.get_params();
	const std::string &trailing = line.get_trailing();
	if ((params.size() < 3) || trailing.empty())
	{
		const std::string msg = ":localhost 461 * USER :Not enough parameters\r\n";
		send(client.get_FD(), msg.c_str(), msg.size(), 0);
		return;
	}
	const std::string &username = params[0];
	const std::string &hostname = params[1];
	const std::string &servername = params[2];
	std::string realname = line.get_trailing(); // trailing part after ':'

	client.set_username(username);
	if (!realname.empty())
		client.set_realname(realname);

	// (HexChat often sends "0 *" here)
	if (hostname != "0" && hostname != "*")
		client.set_hostname(hostname);
	if (servername != "0" && servername != "*")
		client.set_servername(servername);

	server.welcome(client);
}
