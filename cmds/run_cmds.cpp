#include "cmds_list.hpp"
#include "../server/Server.hpp"
#include "../client/Client.hpp"

void run_cmds(Server &server, RawTextLine &line, Client &client)
{
    typedef void (*CmdFunc)(Server &, RawTextLine &, Client &);

    const std::string cmds[MAX_CMDS] = {
        "NICK","USER","PASS","QUIT","PRIVMSG","NOTICE",
        "JOIN","PART","LIST","TOPIC","NAMES","WHO","WHOIS","WHOWAS"
    };

    CmdFunc funcs[MAX_CMDS] = {
        cmd_nick, cmd_user, cmd_pass, cmd_quit, cmd_privmsg, cmd_notice,
        cmd_join, cmd_part, cmd_list, cmd_topic, cmd_names, cmd_who,
        cmd_whois, cmd_whowas
    };


    for (int i = 0; i < MAX_CMDS; i++)
    {
        if (line.get_command() == cmds[i])
        {
            funcs[i](server, line, client);
        }
    }
}

void cmd_quit(Server &server, RawTextLine &line, Client &client) {(void)server; (void)line; (void)client;}
void cmd_notice(Server &server, RawTextLine &line, Client &client) {(void)server; (void)line; (void)client;};

//void cmd_topic(Server &server, RawTextLine &line, Client &client){(void)server; (void)line; (void)client;};
void cmd_names(Server &server, RawTextLine &line, Client &client){(void)server; (void)line; (void)client;};
void cmd_who(Server &server, RawTextLine &line, Client &client){(void)server; (void)line; (void)client;};
void cmd_whois(Server &server, RawTextLine &line, Client &client){(void)server; (void)line; (void)client;};
void cmd_whowas(Server &server, RawTextLine &line, Client &client){(void)server; (void)line; (void)client;};