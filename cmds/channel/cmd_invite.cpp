#include "../../lib_irc.hpp"

void cmd_invite(Server &server, RawTextLine &line, Client &client)
{
	const std::vector<std::string> &params = line.get_params();

	// 1) Validate params
	if (params.size() < 2)
	{
		err_needmoreparams(server.get_servername(), client, "INVITE");
		return;
	}
	const std::string target_cli = params[0];
	std::string channel_name = params[1];
	// if (channel_name[0] != '#' && channel_name[0] != '&' && channel_name[0] != '+' && channel_name[0] != '!')
	// 	channel_name = "#" + channel_name;

	// 2) Find channel
	Channel *chan = server.get_channel(channel_name);
	if (!chan)
	{
		err_nosuchchannel(server.get_servername(), client, channel_name);
		return;
	}

	// 3) Inviter must be on the channel
	if (!chan->has_user(&client))
	{
		err_notonchannel(server.get_servername(), client, chan);
		return;
	}
	// 4) if invite only, is the inviter an operator?
	if (chan->get_mode_i() == true)
	{
		if (!chan->is_operator(&client))
		{
			err_chanoprivsneeded(server.get_servername(), client, chan);
			return;
		}
	}
	// 5) Find target client by nick
	Client *target = server.get_client(target_cli); // add this helper if missing
	if (!target)
	{
		err_nosuchnick(server.get_servername(), client, client.get_nickname(), "INVITE");
		return;
	}

	// 6) If already on channel → error
	if (chan->has_user(target))
	{
		err_useronchannel(server.get_servername(), client, target_cli, channel_name);
		return;
	}

	// send target client the invite message
	std::string user_invite = ":" + client.get_prefix() + " INVITE " + target_cli + " :" + channel_name + CRLF;
	send(target->get_FD(), user_invite.c_str(), user_invite.length(), 0);

	std::string rpl_inviting = ":" + server.get_servername() + " 341 " + client.get_nickname() + " " + target_cli + " " + channel_name + CRLF;
	send(client.get_FD(), rpl_inviting.c_str(), rpl_inviting.length(), 0);
	chan->add_to_invitees(target);
}