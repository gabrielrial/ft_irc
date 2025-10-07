#pragma once

//  PARAMETERS

#define BUFFER_SIZE 1024
#define BUFFER_LINE 510
#define HOST_NAME_SIZE 128
#define CRLF "\r\n"


// CLASS INCLUDE

#include "channel/Channel.hpp"
#include "parser/Parser.hpp"
#include "server/Server.hpp"
#include "client/Client.hpp"
#include "cmds/cmds_list.hpp"
#include "err_rpl/err_rpl.hpp"


// LIBS INCLUDE

//#include "cmd_list.hpp"

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <string>
#include <poll.h>
#include <vector>
#include <map>
#include <stdio.h>
#include <errno.h>
#include <sstream>
#include <fcntl.h>
#include <csignal>
#include <ctime>

// CONFIGS

// #define PORT 6667
#define IP "127.0.0.1"

extern volatile sig_atomic_t g_running;