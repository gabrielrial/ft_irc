#include "../bot_cmds_list.hpp"
#include "../../Bot.hpp"

void cmd_spam(RawTextLine &line, Bot &bot)
{
	(void)line;
	for (int i = 0; i < 1000; i++)
	{
		bot.send_message(bot.get_socket(), "PRIVMSG #general :Spam.");
	}
	return;
}