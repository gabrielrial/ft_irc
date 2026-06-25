#include "../bot_cmds_list.hpp"
#include "../../Bot.hpp"

void cmd_spam(RawTextLine &line, Bot &bot)
{
	(void)line;
	for (size_t i = 0; i < line.get_params().size(); i++)
	{
		std::string target = line.get_params()[i];
		if (line.get_params()[i] == bot.get_nickname())
			target = bot.get_nick_form_prefix(line);

		for (int i = 0; i < 1000; i++)
		{
			bot.send_message(bot.get_socket(), "PRIVMSG "+ target + " :Spam.");
		}
	}
	return;
}