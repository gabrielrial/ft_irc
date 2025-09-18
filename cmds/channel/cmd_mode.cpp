#include "../../lib_irc.hpp"

// In cmds/channel/cmd_mode.cpp
void cmd_mode(Server &server, RawTextLine &line, Client &client);
int check_mode_params(RawTextLine &line, Client &client, std::string server_name);
int check_mode_channel(Server &server, Client &client, std::string channel_name, std::string server_name);

void change_mode(Server &server, Client &client, Channel *channel, 
				const std::string &modes, const std::vector<std::string> &mode_params,
				std::string server_name);

void cmd_mode(Server &server, RawTextLine &line, Client &client)
{
	char server_name[256];
	if (gethostname(server_name, sizeof(server_name)) != 0)
		strcpy(server_name, "localhost");
	if (check_mode_params(line, client, server_name) == 1)
		return;
	const std::vector<std::string> &params = line.get_params();
	std::string channel_name = params[0];
	if (check_mode_channel(server, client, channel_name, server_name) == 1)
		return;
	if (params.size() == 1)
	{
		//RPL_CHANNELMODEIS (324)
		return;
	}
	change_mode(server, client, server.get_channel(channel_name),
				params[1], 
				std::vector<std::string>(params.begin() + 2, params.end()),
				server_name);
}

int check_mode_params(RawTextLine &line, Client &client, std::string server_name)
{
		if (line.get_params().empty())
	{
		std::string err_needmoreparams = ":" + std::string(server_name) + " 461 " + 
				client.get_nickname() + " MODE :Not enough parameters\r\n";
		send(client.get_FD(), err_needmoreparams.c_str(), err_needmoreparams.length(), 0);
		return 1;
	}
	return 0;
}

int check_mode_channel(Server &server, Client &client, 
						std::string channel_name, std::string server_name)
{
	Channel *channel = server.get_channel(channel_name);
	if (!channel)
	{
		std::string err_nosuchchannel = ":" + std::string(server_name) + " 403 " + 
				client.get_nickname() + " " + channel_name + " :No such channel\r\n";
		send(client.get_FD(), err_nosuchchannel.c_str(), err_nosuchchannel.size(), 0);
		return 1;
	}
	else if (!channel->has_user(client))
	{
		std::string err_notonchannel = ":" + std::string(server_name) + " 442 " + 
				client.get_nickname() + " " + channel_name + " :You're not on that channel\r\n";
		send(client.get_FD(), err_notonchannel.c_str(), err_notonchannel.size(), 0);
		return 1;
	}
	else if (!channel->is_operator(client))
	{
		std::string err_chanoprivsneeded = ":" + server_name + " 482 " + 
							client.get_nickname() + " " + channel_name + 
							" :You're not channel operator\r\n";
		send(client.get_FD(), err_chanoprivsneeded.c_str(), err_chanoprivsneeded.length(), 0);
		return;
	}
	return 0;
}

void change_mode(Server &server, Client &client, Channel *channel,
				const std::string &modes, const std::vector<std::string> &mode_params,
				std::string server_name)
{
	bool adding = true;  // + or - mode
	size_t param_index = 0;

	for (char mode : modes) {
		switch (mode) {
			case '+':
				adding = true;
				break;
			case '-':
				adding = false;
				break;
			case 'o': // operator status
				if (param_index < mode_params.size()) {
					// Handle op/deop
					std::string target_nick = mode_params[param_index++];
					// Find target client and add/remove operator status
				}
				break;
			// Add other mode cases
		}
	}
	
	// Broadcast mode changes to channel
	std::string mode_msg = ":" + client.get_prefix() + " MODE " + channel->get_name() + " " + modes;
	// Add parameters and broadcast
}

/*
Success responses:
RPL_CHANNELMODEIS (324)  // Current channel modes
RPL_BANLIST (367)        // If implementing ban lists
RPL_ENDOFBANLIST (368)   // End of ban list

Error responses:
ERR_NEEDMOREPARAMS (461)    // Not enough parameters
ERR_CHANOPRIVSNEEDED (482)  // Not channel operator
ERR_NOSUCHCHANNEL (403)     // Channel doesn't exist
ERR_NOTONCHANNEL (442)      // User not on channel
ERR_UNKNOWNMODE (472)       // Unknown mode character
*/