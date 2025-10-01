#pragma once

#include "../../lib_irc.hpp"

class Bot;

void bot_run_cmds(RawTextLine &line, Bot &bot);

void cmd_joke(RawTextLine &line, Bot &bot);

void cmd_invite(RawTextLine &line, Bot &bot);

void cmd_spam(RawTextLine &line, Bot &bot);

void bot_filter(RawTextLine &line, Bot &bot);

void bot_privmsg(RawTextLine &line, Bot &bot);