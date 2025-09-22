#include "../../lib_irc.hpp"

void cmd_user(Server &server, RawTextLine &line, Client &client)
{
    if (client.is_registered()) // Already registered → ERR_ALREADYREGISTRED
    {
        const std::string msg = ":localhost 462 * :You may not reregister\r\n";
		send(client.get_FD(), msg.c_str(), msg.size(), 0);
		return;
    }
    if (!client.has_pass()) // Require PASS to have succeeded first
    {
        const std::string msg = ":localhost 464 * :Password incorrect\r\n";
		send(client.get_FD(), msg.c_str(), msg.size(), 0);
		server.schedule_close(client.get_FD());
		return;
    }
    const std::vector<std::string>& params = line.get_params();
	const std::string& trailing = line.get_trailing();
	if ((params.size() < 3) || trailing.empty())
	{
		const std::string msg = ":localhost 461 * USER :Not enough parameters\r\n";
		send(client.get_FD(), msg.c_str(), msg.size(), 0);
		return;
	}
    const std::string& username = params[0];
	const std::string& hostname = params[1];
	const std::string& servername = params[2];
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
	{
		server.welcome(client);
		server.set_client_amt();

		std::cout << "Nickname: " << client.get_nickname() << std::endl;
		std::cout << "Username: " << client.get_username() << std::endl;
		std::cout << "Hostname: " << client.get_hostname() << std::endl;
		std::cout << "Servername: " << client.get_servername() << std::endl;
		std::cout << "Realname: " << client.get_realname() << std::endl;
		std::cout << "===============================================" << std::endl;
	}
}
