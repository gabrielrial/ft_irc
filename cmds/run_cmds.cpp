#include "cmds_list.hpp"

bool run_cmd(RawTextLine &line, Client &client)
{
	typedef void (*CmdFunc)(const std::string &);

	const std::string cmds[14] =
		{"NICK",
		 "USER",
		 "PASS",
		 "QUIT",
		 "PRIVMSG",
		 "NOTICE",
		 "JOIN",
		 "PART",
		 "LIST",
		 "TOPIC",
		 "NAMES",
		 "WHO",
		 "WHOIS",
		 "WHOWAS"};

	CmdFunc funcs[14] = {
		cmdNick,
		cmdUser,
		cmdPass,
		cmdQuit,
		cmdPrivmsg,
		cmdNotice,
		cmdJoin,
		cmdPart,
		cmdList,
		cmdTopic,
		cmdNames,
		cmdWho,
		cmdWhois,
		cmdWhowas};

	for (int i = 0; i < MAX_CMDS; i++)
	{
		if (line.getCommand() == cmds[i])
		{
			(this->*funcs[i])(client, line);
			return;
		}
	}
}