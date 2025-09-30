#include "../bot_cmds_list.hpp"
#include "../../Bot.hpp"

void cmd_invite(RawTextLine &line, Bot &bot)
{
    bot.send_message(bot.get_socket(), "JOIN " + line.get_trailing());
}