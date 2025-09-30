#include "cmds_list.hpp"
#include "../server/Server.hpp"
#include "../client/Client.hpp"

void run_cmds(Server &server, RawTextLine &line, Client &client)
{
	typedef void (*CmdFunc)(Server &, RawTextLine &, Client &);

	static const std::string cmds[] =
		{
			"NICK", "USER", "PASS", "QUIT", "PRIVMSG", 
			"NOTICE", "JOIN", "PART", "INVITE", "LIST",
			"TOPIC", "NAMES", "WHO", "MODE", "KICK"};

	static CmdFunc funcs[] =
		{
			cmd_nick, cmd_user, cmd_pass, cmd_quit, cmd_privmsg,
			cmd_notice, cmd_join, cmd_part, cmd_invite, cmd_list,
			cmd_topic, cmd_names, cmd_who, cmd_mode, cmd_kick};

	for (int i = 0; i < MAX_CMDS; i++)
	{
		if (!client.is_registered())
		{
			if ((line.get_command() == "PASS"))
			{
				funcs[2](server, line, client);
				return;
			}
			else if (line.get_command() == "USER")
			{
				funcs[1](server, line, client);
				return;
			}
			else if (line.get_command() == "NICK")
			{
				funcs[0](server, line, client);
				return;
			}
			else
			{
				std::string welcome = server.get_servername() + ": You must registrate first. (PASS, NICK and USER are requiered)\r\n";
				send(client.get_FD(), welcome.c_str(), welcome.size(), 0);
				return;
			}
		}
		else if (client.is_registered() && line.get_command() == cmds[i])
			funcs[i](server, line, client);
	}
}
