#pragma once

#include "../lib_irc.hpp"

class Server;
class Client;
class RawTextLine;

#define MAX_CMDS 15

void run_cmds(Server &server, RawTextLine &line, Client &client);

/* CONNECTION
 *
 * NICK <nickname> → sets your nickname.
 * USER <username> <hostname> <servername> <realname> → identifies the user when connecting.
 * PASS <password> → Checks if the client has entered the password.
 * QUIT [<message>] → disconnects from the server with an optional message.
 * */

void cmd_nick(Server &server, RawTextLine &line, Client &client);
void cmd_user(Server &server, RawTextLine &line, Client &client);
void cmd_pass(Server &server, RawTextLine &line, Client &client);
void cmd_quit(Server &server, RawTextLine &line, Client &client);

/* MESSAGING
 *
 * PRIVMSG <target> <message> → sends a message to a user or a channel.
 * NOTICE	<target> <message> → like PRIVMSG but usually for automated messages (cannot trigger auto-replies).
 * */

void cmd_privmsg(Server &server, RawTextLine &line, Client &client);
void cmd_notice(Server &server, RawTextLine &line, Client &client);

/* CHANNEL
 *
 * JOIN <channel> → join (or create) a channel.
 * PART <channel> [<message>] → leave a channel.
 * LIST [<channel>] → list channels (and their topics).
 * INVITE <nickname> <channel> → invites a user to a channel.
 * TOPIC <channel> [<topic>] → view or set a channel’s topic.
 * NAMES [<channel>] → shows who is currently in the channel.
 * */

void cmd_join(Server &server, RawTextLine &line, Client &client);
void cmd_part(Server &server, RawTextLine &line, Client &client);
void cmd_list(Server &server, RawTextLine &line, Client &client);
void cmd_invite(Server &server, RawTextLine &line, Client &client);
void cmd_topic(Server &server, RawTextLine &line, Client &client);
void cmd_names(Server &server, RawTextLine &line, Client &client);
void cmd_mode(Server &server, RawTextLine &line, Client &client);
void cmd_kick(Server &server, RawTextLine &line, Client &client);

/* USER INFO
 *
 * WHO [<name>] → list users matching a name/channel.
 * WHOIS <nick> → detailed info about a user.
 * WHOWAS <nick> → info about a disconnected nickname.
 * */

void cmd_who(Server &server, RawTextLine &line, Client &client);
// void cmd_whois(Server &server, RawTextLine &line, Client &client);
// void cmd_whowas(Server &server, RawTextLine &line, Client &client);
void cmd_name(Server &server, RawTextLine &line, Client &client);
void cmd_name_join(Server &server, std::string &channel_name);