#pragma once
#include "../../lib_irc.hpp"

void send_message_to_channel(Channel *channel, Client &sender,
							 const std::string &targetName, const std::string &message,
							 const std::string &command, bool sendErrors, Server &server);

void send_message_to_user(Server &server, Client &sender,
						  const std::string &targetName, const std::string &message,
						  const std::string &command, bool sendErrors);

void send_message(Server &server, Client &sender,
				  const std::vector<std::string> &targets, const std::string &message,
				  const std::string &command, bool sendErrors);

std::vector<std::string> get_target(RawTextLine &line);
