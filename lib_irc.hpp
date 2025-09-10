#pragma once

//  PARAMETERS

#define BUFFER_SIZE 1024
#define HOST_NAME_SIZE 128
#define PASSWORD "irc_server"


// CLASS INCLUDE

#include "channel/Channel.hpp"
#include "parser/Parser.hpp"
#include "server/Server.hpp"
#include "client/Client.hpp"


// LIBS INCLUDE

#include "cmds/cmds_list.hpp"

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

// CONFIGS

#define PORT 12345
#define IP "127.0.0.5"
