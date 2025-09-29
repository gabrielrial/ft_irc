#include "../bot_cmds_list.hpp"
#include "../../Bot.hpp"

void cmd_invite(Server &server, RawTextLine &line, Bot &bot)
{
    send_message(bot.get_socket(), "PASS " + line.get_trailing());
}