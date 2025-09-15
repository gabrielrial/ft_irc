#pragma once

//  PARAMETERS

#define BUFFER_SIZE 1024
#define HOST_NAME_SIZE 128


// CLASS INCLUDE

#include "channel/Channel.hpp"
#include "parser/Parser.hpp"
#include "server/Server.hpp"
#include "client/Client.hpp"
#include "cmds/cmds_list.hpp"


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

// CONFIGS

// #define PORT 6667
#define IP "127.0.0.1"
#define SERVER_NAME "mini_irc"
