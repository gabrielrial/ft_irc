#include "bot_cmds_list.hpp"


void bot_run_cmds(Server &server, RawTextLine &line, Client &client)
{
    typedef void (*CmdFunc)(Server &, RawTextLine &, Client &);

    static const std::string cmds[] =
        {
            "!joke", "!time", "INVITE"
        };

    static CmdFunc funcs[] =
        {
            cmd_joke, cmd_joke, cmd_invite
        };

    for (int i = 0; i < MAX_CMDS; i++)
    {
        if (line.get_command() == cmds[i])
            funcs[i](server, line, client);
    }
}

