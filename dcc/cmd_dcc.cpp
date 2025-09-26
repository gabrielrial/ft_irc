#include "cmd_dcc.hpp"
#include "DCCManager.hpp"
#include "DCCTransfer.hpp"
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <ctime>

void cmd_dcc(Server &server, RawTextLine &line, Client &client)
{
    if (line.get_params().size() < 3)
    {
        err_needmoreparams(server.get_servername(), client, "DCC");
        return;
    }

    std::string subcommand = line.get_params()[0];
    std::string target = line.get_params()[1];
    std::string filename = line.get_params()[2];

    if (subcommand == "SEND")
    {
        server.get_dcc_manager().handle_dcc_send(client, target, filename);
    }
    else if (subcommand == "ACCEPT")
    {
        // Note: Changed from SEND to ACCEPT handler
        server.get_dcc_manager().handle_dcc_accept(client, target, filename);
    }
}