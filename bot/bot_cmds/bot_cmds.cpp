#include "bot_cmds_list.hpp"


void bot_run_cmds(RawTextLine &line, Bot &bot)
{
    typedef void (*BotFunc)(RawTextLine &, Bot &);

    static const std::string cmds[3] =
        {
            "!joke", "!time", "INVITE"
        };

    static BotFunc funcs[] =
        {
            cmd_joke, cmd_invite, cmd_invite
        };

    for (int i = 0; i < 3; i++)
    {
        if (line.get_command() == cmds[i])
            funcs[i](line, bot);
    }
}

