#include "../../lib_irc.hpp"
#include "messaging_utils.hpp"

void cmd_privmsg(Server &server, RawTextLine &line, Client &client)
{
	std::vector<std::string> targets = get_target(line);
	std::string message = line.get_trailing();

	send_message(server, client, targets, message, "PRIVMSG", true);
}
