#pragma once

class Server;
class Client;
class RawTextLine;

#define MAX_CMDS 14

// Declaraciones
void run_cmds(Server &server, RawTextLine &line, Client &client);

void cmdNick(Server &server, RawTextLine &line, Client &client);
void cmdUser(Server &server, RawTextLine &line, Client &client);
void cmdPass(Server &server, RawTextLine &line, Client &client);
void cmdQuit(Server &server, RawTextLine &line, Client &client);
void cmdPrivmsg(Server &server, RawTextLine &line, Client &client);
void cmdNotice(Server &server, RawTextLine &line, Client &client);
void cmdJoin(Server &server, RawTextLine &line, Client &client);
void cmdPart(Server &server, RawTextLine &line, Client &client);
void cmdList(Server &server, RawTextLine &line, Client &client);
void cmdTopic(Server &server, RawTextLine &line, Client &client);
void cmdNames(Server &server, RawTextLine &line, Client &client);
void cmdWho(Server &server, RawTextLine &line, Client &client);
void cmdWhois(Server &server, RawTextLine &line, Client &client);
void cmdWhowas(Server &server, RawTextLine &line, Client &client);

