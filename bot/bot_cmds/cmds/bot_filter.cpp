#include "../bot_cmds_list.hpp"
#include "../../Bot.hpp"

void bot_filter(RawTextLine &line, Bot &bot)
{
	std::string channel = line.get_params()[0];
	if (channel.empty() || (channel[0] != '#' && channel[0] != '&' && channel[0] != '+' && channel[0] != '!'))
		return;

	std::string client = bot.get_nick_form_prefix(line);
	if (client.empty())
		return;
	
	bot.send_message(bot.get_socket(), "NAME " + channel);
	RawTextLine names = bot.get_answer();

	if (names.get_trailing().find("@" + client) == std::string::npos)
		return;

	if (!bot.get_filter())
	{
		bot.set_filter();
		bot.send_message(bot.get_socket(), "NOTICE " + client + " :Bot filter mode has been activated on channel " + channel + ". Be sure you are part of the channel operators.");
	}
	else
	{
		bot.set_filter();
		bot.send_message(bot.get_socket(), "NOTICE " + client + " :Bot filter mode has been deactivated on channel " + channel + ".");
	}
}