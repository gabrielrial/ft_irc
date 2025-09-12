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
        cmdNick, cmdUser, cmdPass, cmdQuit, cmdPrivmsg, cmdNotice,
        cmdJoin, cmdPart, cmdList, cmdTopic, cmdNames, cmdWho,
        cmdWhois, cmdWhowas
    };


    for (int i = 0; i < MAX_CMDS; i++)
    {
        if (line.getCommand() == cmds[i])
        {
            funcs[i](server, line, client);
        }
    }
}
void cmdNick(Server &server, RawTextLine &line, Client &client) {(void)server; (void)line; (void)client;}
void cmdUser(Server &server, RawTextLine &line, Client &client) {(void)server; (void)line; (void)client;}
void cmdPass(Server &server, RawTextLine &line, Client &client) {(void)server; (void)line; (void)client;}
void cmdQuit(Server &server, RawTextLine &line, Client &client) {(void)server; (void)line; (void)client;}
void cmdNotice(Server &server, RawTextLine &line, Client &client) {(void)server; (void)line; (void)client;};
//void cmdJoin(Server &server, RawTextLine &line, Client &client){(void)server; (void)line; (void)client;};
void cmdPart(Server &server, RawTextLine &line, Client &client){(void)server; (void)line; (void)client;};
void cmdList(Server &server, RawTextLine &line, Client &client){(void)server; (void)line; (void)client;};
void cmdTopic(Server &server, RawTextLine &line, Client &client){(void)server; (void)line; (void)client;};
void cmdNames(Server &server, RawTextLine &line, Client &client){(void)server; (void)line; (void)client;};
void cmdWho(Server &server, RawTextLine &line, Client &client){(void)server; (void)line; (void)client;};
void cmdWhois(Server &server, RawTextLine &line, Client &client){(void)server; (void)line; (void)client;};
void cmdWhowas(Server &server, RawTextLine &line, Client &client){(void)server; (void)line; (void)client;};