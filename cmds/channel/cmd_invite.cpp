#include "../../lib_irc.hpp"

void cmd_invite(Server &server, RawTextLine &line, Client &client)
{
    const std::vector<std::string> &params = line.get_params();

    char server_name[256];
    // 1) Validate params
	if (params.size() < 2)
	{
		if (gethostname(server_name, sizeof(server_name)) != 0) strcpy(server_name, "localhost");
		std::string err = ":" + std::string(server_name) + " 461 " + client.get_nickname() + " INVITE :Not enough parameters\r\n";
		send(client.get_FD(), err.c_str(), err.length(), 0);
		return;
	}
    const std::string target_cli = params[0];
	std::string channel_name = params[1];
	if (channel_name[0] != '#' && channel_name[0] != '&' && channel_name[0] != '+' && channel_name[0] != '!')
		channel_name = "#" + channel_name;

	if (gethostname(server_name, sizeof(server_name)) != 0)
        strcpy(server_name, "localhost");

    // 2) Find channel
	Channel *chan = server.get_channel(channel_name);
	if (!chan)
	{
		std::string err = ":" + std::string(server_name) + " 403 " + client.get_nickname() + " " + channel_name + " :No such channel\r\n";
		send(client.get_FD(), err.c_str(), err.length(), 0);
		return;
	}

	// 3) Inviter must be on the channel
	if (!chan->has_user(client))
	{
		std::string err = ":" + std::string(server_name) + " 442 " + client.get_nickname() + " " + channel_name + " :You're not on that channel\r\n";
		send(client.get_FD(), err.c_str(), err.length(), 0);
		return;
	}
    // 4) if invite only, is the inviter an operator?
	if (chan->get_mode_i() == true)
    {
        if (!chan->is_operator(client))
        {
            std::string err = ":" + std::string(server_name) + " 482 " + client.get_nickname() + " " + channel_name + " :You're not channel operator\r\n";
            send(client.get_FD(), err.c_str(), err.length(), 0);
            return;
        }
    }
    // 5) Find target client by nick
    Client *target = server.get_client(target_cli); // add this helper if missing
    if (!target)
    {
        std::string err = ":" + std::string(server_name) + " 401 " + client.get_nickname() + " " + target_cli + " :No such nickname\r\n";
        send(client.get_FD(), err.c_str(), err.length(), 0);
        return;
    }

    // 6) If already on channel → error
	if (chan->has_user(*target))
	{
		std::string err = ":" + std::string(server_name) + " 443 " + client.get_nickname() + " " + target_cli + " " + channel_name + " :is already on channel\r\n";
		send(client.get_FD(), err.c_str(), err.length(), 0);
		return;
	}

    // send target client the invite message
	std::string user_invite = ":" + client.get_prefix() + " INVITE " + target_cli + " :" + channel_name + CRLF;
	send(target->get_FD(), user_invite.c_str(), user_invite.length(), 0);

	std::string rpl_inviting = ":" + std::string(server_name) + " 341 " + client.get_nickname() + " " + target_cli + " " + channel_name + CRLF;
	send(client.get_FD(), rpl_inviting.c_str(), rpl_inviting.length(), 0);
	chan->add_to_invitees(target);
}