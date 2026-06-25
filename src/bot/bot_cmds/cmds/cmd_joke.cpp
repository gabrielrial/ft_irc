#include "../bot_cmds_list.hpp"
#include "../../Bot.hpp"
#include <vector>
#include <string>
#include <cstdlib> // rand, srand
#include <ctime>   // time

void cmd_joke(RawTextLine &line, Bot &bot)
{
	static std::vector<std::string> jokes;
	if (jokes.empty())
	{
		jokes.push_back("My boyfriend dumped me, so I stole his chairwheel... guess who came crawling back...");
		jokes.push_back("Error 404: Good joke not found.");
		jokes.push_back("Parallel lines have so much in common. Too bad they’ll never meet.");
	}

	int index = std::rand() % jokes.size();

	for (size_t i = 0; i < line.get_params().size(); i++)
	{
		std::string target = line.get_params()[i];
		if (line.get_params()[i] == bot.get_nickname())
			target = bot.get_nick_form_prefix(line);

		bot.send_message(
			bot.get_socket(),
			"PRIVMSG " + target + " :" + jokes[index]);
	}
}
