#include "cmds_list.hpp"
#include "../server/Server.hpp"
#include "../client/Client.hpp"

void run_cmds(Server &server, RawTextLine &line, Client &client)
{
    if (client.has_pass() == false)
    {
        if (line.get_command() == "PASS")
        {
            cmd_pass(server, line, client);
        }
        return ;
    }

    typedef void (*CmdFunc)(Server &, RawTextLine &, Client &);

    const std::string cmds[MAX_CMDS] = {
        "NICK","USER","PASS","QUIT","PRIVMSG","NOTICE",
        "JOIN","PART", "INVITE", "LIST","TOPIC","NAMES","WHO","NAME","WHOWAS",
		"MODE"
    };

    CmdFunc funcs[MAX_CMDS] = {
        cmd_nick, cmd_user, cmd_pass, cmd_quit, cmd_privmsg, cmd_notice,
        cmd_join, cmd_part, cmd_invite, cmd_list, cmd_topic, cmd_names, cmd_who,
        cmd_name, cmd_whowas, cmd_mode
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

void cmd_whois(Server &server, RawTextLine &line, Client &client){(void)server; (void)line; (void)client;};
void cmd_whowas(Server &server, RawTextLine &line, Client &client){(void)server; (void)line; (void)client;};