#include "../lib_irc.hpp"

void cmdPrivmsg(Server &server, RawTextLine &line, Client &client)
{
    std::vector<Client*> client_list;
    server.check_client(line, client_list);

    if (client_list.empty())
        return;

    for (size_t i = 0; i < client_list.size(); ++i)
    {
        Client* target = client_list[i];
        std::string msg = std::string(":") + client.getNickname() + " PRIVMSG " + target->getUsername() + " :" + line.getTrailing() + "\r\n";
        std::cout << msg << std::endl;
       // std::string msg1 = std::string(":") + target->getNickname() + " PRIVMSG " + target->getUsername() + " :" + line.getTrailing() + "\r\n";

        send(target->getFd(), msg.c_str(), msg.size(), 0);
       // send(client.getFd(), msg1.c_str(), msg1.size(), 0);
    }
}