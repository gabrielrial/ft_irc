#include "bot_cmds_list.hpp"

void bot_run_cmds(RawTextLine &line, Bot &bot)
{
	typedef void (*BotFunc)(RawTextLine &, Bot &);

	static const std::string cmds[2] =
		{
			"INVITE", "PRIVMSG"};

	static BotFunc funcs[] =
		{
			cmd_invite, bot_privmsg
		};

	for (int i = 0; i < 3; i++)
	{
		std::cout << "cmd :" << line.get_command() << std::endl;
		if (line.get_command() == cmds[i])
			funcs[i](line, bot);
	}
}
