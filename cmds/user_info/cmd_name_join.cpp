#include "../../lib_irc.hpp"

void cmd_name_join(Channel &channel, Client &client)
{
    std::vector<Client> client_list = channel.get_users();

    std::string rpl_namreply = ":" + server.get_servername() + " 353 " +
                               client.get_nickname() + " = " + channel->get_name() + " :";

    std::string names_list;
    for (size_t i = 0; i < client_list.size(); i++)
    {
        std::string client_name = client_list[i].get_nickname();

        if (channel->is_operator(client_name))
            client_name = "@" + client_name;

        if ((rpl_namreply + names_list + client_name).size() < 510)
        {
            if (!names_list.empty())
                names_list += " ";
            names_list += client_name;
        }
        else
        {
            std::string msg = rpl_namreply + names_list + CRLF;
            send(client.get_FD(), msg.c_str(), msg.size(), 0);
            names_list = client_name;
            continue;
        }
        if (client_list.size() - 1 == i)
        {
            std::string msg = rpl_namreply + names_list + CRLF;
            send(client.get_FD(), msg.c_str(), msg.size(), 0);
            names_list = client_name;
        }
    }

    std::string endmsg = ":" + server.get_servername() + " 366 " +
                         client.get_nickname() + " " + channel->get_name() +
                         " :End of NAMES list" + CRLF;
    send(client.get_FD(), endmsg.c_str(), endmsg.size(), 0);
}
}