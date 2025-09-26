#pragma once

#include "../../lib_irc.hpp"

void run_cmds(Server &server, RawTextLine &line, Client &client);

void cmd_joke(Server &server, RawTextLine &line, Client &client);