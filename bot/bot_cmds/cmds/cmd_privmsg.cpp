#include "../bot_cmds_list.hpp"
#include "../../Bot.hpp"

void bot_privmsg(RawTextLine &line, Bot &bot)
{
	std::cout << "trailing: " << line.get_trailing() << std::endl;
	if (line.get_trailing() == "!joke")
		cmd_joke(line, bot);
	else if (line.get_trailing() == "!time")
		cmd_time(line, bot);
	else if (line.get_trailing() == "!help")
	{
		for (size_t i = 0; i < line.get_params().size(); i++)
		{
			if (line.get_params()[i] == bot.getNickname())
			{
				size_t start = 0; // saltar ':' inicial
				size_t end = line.get_prefix().find('!', start);

				std::string nick;
				if (end != std::string::npos)
				{
					nick = line.get_prefix().substr(start, end - start);
				}
				bot.send_message(bot.get_socket(), "PRIVMSG " + nick + " :AAAA");
			}
			else
				bot.send_message(bot.get_socket(), "PRIVMSG " + line.get_params()[i] + " :I only work with !joke and !time commands. You can also invite me to channels.");
		}
	}
}