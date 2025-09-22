#include "cmds_list.hpp"
#include "../server/Server.hpp"
#include "../client/Client.hpp"

void run_cmds(Server &server, RawTextLine &line, Client &client)
{
	const std::string &cmd = line.get_command();

	typedef void (*CmdFunc)(Server &, RawTextLine &, Client &);

    static const std::string all_cmds[] =
	{
        "NICK","USER","PASS","QUIT","PRIVMSG","NOTICE",
        "JOIN","PART", "INVITE", "LIST","TOPIC","NAMES","WHO"
    };

     static CmdFunc funcs[] =
	{
        cmd_nick, cmd_user, cmd_pass, cmd_quit, cmd_privmsg, cmd_notice,
        cmd_join, cmd_part, cmd_invite, cmd_list, cmd_topic, cmd_names, cmd_who,
    };

	if (!client.is_registered())
	{
		for (size_t i = 0; i < 3; i++)
		{
			if (cmd == all_cmds[i])
			{
				funcs[i](server, line, client);
				return;
			}
			if (cmd == "CAP") return ;
		}
		const std::string msg = ":localhost 451 * :You have not registered\r\n";
		send(client.get_FD(), msg.c_str(), msg.size(), 0);
		return;
	}
	for (int i = 0; i < MAX_CMDS; i++)
	{
		if (cmd == all_cmds[i])
		{
			funcs[i](server, line, client);
		}
	}
}

void cmd_notice(Server &server, RawTextLine &line, Client &client) {(void)server; (void)line; (void)client;};

//void cmd_topic(Server &server, RawTextLine &line, Client &client){(void)server; (void)line; (void)client;};
void cmd_names(Server &server, RawTextLine &line, Client &client){(void)server; (void)line; (void)client;};

// void cmd_whois(Server &server, RawTextLine &line, Client &client){(void)server; (void)line; (void)client;}; // unnecessary/extra
// void cmd_whowas(Server &server, RawTextLine &line, Client &client){(void)server; (void)line; (void)client;};// unnecessary/extra