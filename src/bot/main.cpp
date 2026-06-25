#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "../lib_irc.hpp"
#include "Bot.hpp"

int main()
{
	Bot bot;

	bot.init_connection();
	
	if (bot.run_bot() == -1)
		return (-1);

	return 0;
}
