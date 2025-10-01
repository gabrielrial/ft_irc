#include "../bot_cmds_list.hpp"
#include "../../Bot.hpp"

void bot_filter(RawTextLine &line, Bot &bot)
{
	std::cout << "Filter function" << std::endl;
	std::string channel = line.get_params()[0];
	std::cout << "channel: "<< channel << std::endl;
	if (channel.empty() || (channel[0] != '#' && channel[0] != '&' && channel[0] != '+' && channel[0] != '!'))
	{
		std::cout << "channel not found" << std::endl;
		return;
	}

	std::string client = bot.get_nick_form_prefix(line);
	std::cout << "client: "<< client << std::endl;
	if (client.empty())
		return;

	
	bot.send_message(bot.get_socket(), "NAME " + channel);
	std::cout << "enviando /NAME " + channel << std::endl;
	RawTextLine names = bot.get_answer();
	std::cout << names.get_command() << std::endl;
	std::cout << names.get_prefix() << std::endl;

	std::cout << "end of bot_filter" << std::endl;
	if (names.get_trailing().find("@" + client) == std::string::npos)
		return;
	
	std::cout << "end of bot_filter" << std::endl;

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
	std::cout << "end of bot_filter" << std::endl;
}