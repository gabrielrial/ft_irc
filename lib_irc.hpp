#ifndef LIB_IRC_HPP
#define LIB_IRC_HPP

//  PARAMETERS

#define BUFFER_SIZE 1024
#define HOST_NAME_SIZE 128
#define PASSWORD "irc_server"


// CLASS INCLUDE

#include "Client.hpp"
#include "Server.hpp"


// LIBS INCLUDE

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

// CONFIGS

#define PORT 54000
#define IP "127.0.0.2"

#endif