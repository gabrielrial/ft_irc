#include "../bot_cmds_list.hpp"
#include "../../Bot.hpp"

void bot_pong(RawTextLine &line, Bot &bot)
{
    std::string pass;

    pass = line.get_params()[0];
    bot.send_message(bot.get_socket(),
							 "PONG " + pass);
}