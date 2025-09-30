#pragma once

#include "../../lib_irc.hpp"

class Bot;

void bot_run_cmds(RawTextLine &line, Bot &bot);

void cmd_joke(RawTextLine &line, Bot &bot);

void cmd_invite(RawTextLine &line, Bot &bot);

void cmd_time(RawTextLine &line, Bot &bot);