#pragma once

#include "../lib_irc.hpp"

#define MAX_CMDS 14

bool run_cmds(RawTextLine &line, Client &client);

/* CONNECTION | REGISTRATION **********************************************
 *
 * NICK <nickname> → sets your nickname.
 * USER <username> <hostname> <servername> <realname> → identifies the user when connecting.
 * PASS <password> → (optional) if the server requires a password
 * QUIT [<message>] → disconnects from the server with an optional message.
 * */

void cmdNick(RawTextLine &line, Client &client);
void cmdUser(RawTextLine &line, Client &client);
void cmdPass(RawTextLine &line, Client &client);
void cmdQuit(RawTextLine &line, Client &client);

/* MESSAGING **************************************************************
 *
 * PRIVMSG <target> <message> → sends a message to a user or a channel.
 * NOTICE	<target> <message> → like PRIVMSG but usually for automated messages (cannot trigger auto-replies).
 * */

void cmdPrivmsg(RawTextLine &line, Client &client);
void cmdNotice(RawTextLine &line, Client &client);

/* CHANNEL ****************************************************************
 *
 * JOIN <channel> → join (or create) a channel.
 * PART <channel> [<message>] → leave a channel.
 * LIST [<channel>] → list channels (and their topics).
 * TOPIC <channel> [<topic>] → view or set a channel’s topic.
 * NAMES [<channel>] → shows who is currently in the channel.
 * */

void cmdJoin(RawTextLine &line, Client &client);
void cmdPart(RawTextLine &line, Client &client);
void cmdList(RawTextLine &line, Client &client);
void cmdTopic(RawTextLine &line, Client &client);
void cmdNames(RawTextLine &line, Client &client);

/* USER INFO **************************************************************
 *
 * WHO [<name>] → list users matching a name/channel.
 * WHOIS <nick> → detailed info about a user.
 * WHOWAS <nick> → info about a disconnected nickname.
 * */

void cmdWho(RawTextLine &line, Client &client);
void cmdWhois(RawTextLine &line, Client &client);
void cmdWhowas(RawTextLine &line, Client &client);
