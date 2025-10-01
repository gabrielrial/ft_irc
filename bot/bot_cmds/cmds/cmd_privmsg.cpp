#include "../bot_cmds_list.hpp"
#include "../../Bot.hpp"

void bot_privmsg(RawTextLine &line, Bot &bot)
{
	if (line.get_trailing() == "!joke")
		cmd_joke(line, bot);
	else if (line.get_trailing() == "!spam")
		cmd_spam(line, bot);
	else if (line.get_trailing() == "!filter")
		bot_filter(line, bot);
	else if (line.get_trailing() == "!help")
	{
		for (size_t i = 0; i < line.get_params().size(); i++)
		{
			std::string client = line.get_params()[i];
			if (line.get_params()[i] == bot.get_nickname())
				client = bot.get_nick_form_prefix(line);
			bot.send_message(bot.get_socket(),
							 "PRIVMSG " + client + " :I only work with !joke, !spam and !filter commands. You can also invite me to channels.");
		}
	}

	if (bot.get_filter())
	{
		if (line.get_trailing().find("fuck") != std::string::npos)
		{
			bot.send_message(bot.get_socket(),
							 "KICK " + line.get_params()[0] + " " +
								 bot.get_nick_form_prefix(line) + " :" +
								 bot.get_nick_form_prefix(line) + " has been kicked for bad word");
		}
	}
}
