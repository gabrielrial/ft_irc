#include "cmds_list.hpp"
#include "../server/Server.hpp"
#include "../client/Client.hpp"

void run_cmds(Server &server, RawTextLine &line, Client &client)
{
	typedef void (*CmdFunc)(Server &, RawTextLine &, Client &);

    static const std::string cmds[] =
	{
        "NICK","USER","PASS","QUIT","PRIVMSG","NOTICE",
        "JOIN","PART", "INVITE", "LIST","TOPIC","NAMES","WHO","NAME",
		"MODE"
    };

     static CmdFunc funcs[] =
	{
        cmd_nick, cmd_user, cmd_pass, cmd_quit, cmd_privmsg, cmd_notice,
        cmd_join, cmd_part, cmd_invite, cmd_list, cmd_topic, cmd_names, cmd_who,
        cmd_name, cmd_mode
    };

    for (int i = 0; i < MAX_CMDS; i++)
    {
        if (line.get_command() == cmds[i])
        {
            funcs[i](server, line, client);
        }
    }
}

void cmd_names(Server &server, RawTextLine &line, Client &client){(void)server; (void)line; (void)client;};
