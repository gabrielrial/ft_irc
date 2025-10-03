#include "../../lib_irc.hpp"

void cmd_user(Server &server, RawTextLine &line, Client &client)
{
	(void)server;
	if (client.is_registered()) // Already registered → ERR_ALREADYREGISTRED (h -> shouldn't this be client.has_user() ?)
	{
		err_alreadyregistered(server.get_servername(), client);
		return;
	}
	const std::vector<std::string> &params = line.get_params();
	const std::string &trailing = line.get_trailing();
	if ((params.size() < 3) || trailing.empty())
	{
		err_needmoreparams(server.get_servername(), client, "USER");
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

	if (client.is_registered())
		server.welcome(client);
}
