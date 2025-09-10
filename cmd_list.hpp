#ifndef CMD_LIST_HPP
#define CMD_LIST_HPP

/* CONNECTION / REGISTRATION **********************************************/
/*
/* NICK <nickname> → sets your nickname.
/* USER <username> <hostname> <servername> <realname> → identifies the user when connecting.
/* PASS <password> → (optional) if the server requires a password
/* QUIT [<message>] → disconnects from the server with an optional message.
/* */

#define NICK
#define USER
#define PASS
#define QUIT

/**************************************************************************/

/* MESSAGING **************************************************************/
/*
/* PRIVMSG <target> <message> → sends a message to a user or a channel.
/* NOTICE	<target> <message> → like PRIVMSG but usually for automated messages (cannot trigger auto-replies).
/* */

#define PRIVMSG
#define NOTICE

/**************************************************************************/

/* CHANNEL ****************************************************************/
/*
/* JOIN <channel> → join (or create) a channel.
/* PART <channel> [<message>] → leave a channel.
/* LIST [<channel>] → list channels (and their topics).
/* TOPIC <channel> [<topic>] → view or set a channel’s topic.
/* NAMES [<channel>] → shows who is currently in the channel.
/* */

#define JOIN
#define PART
#define LIST
#define TOPIC
#define NAMES

/**************************************************************************/

/* USER INFO **************************************************************/
/*
/* WHO [<name>] → list users matching a name/channel.
/* WHOIS <nick> → detailed info about a user.
/* WHOWAS <nick> → info about a disconnected nickname.
/* */

#define WHO
#define WHOIS
#define WHOWAS

/**************************************************************************/

#endif