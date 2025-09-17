#include "../../lib_irc.hpp"

//void cmd_privmsg(Server &server, RawTextLine &line, Client &client)
//{
//	std::vector<Client *> client_list;
//
//	server.check_client(line, client_list);
//
//	if (client_list.empty())
//		return;
//
//	for (size_t i = 0; i < client_list.size(); ++i)
//	{
//		Client *target = client_list[i];
//		std::string msg = std::string(":") + client.get_nickname() + " PRIVMSG " + target->get_username() + " :" + line.get_trailing() + "\r\n";
//		std::cout << msg << std::endl;
//
//		send(target->get_FD(), msg.c_str(), msg.size(), 0);
//	}
//}
std::vector<std::string> get_target(RawTextLine &line);

void cmd_privmsg(Server &server, RawTextLine &line, Client &sender)
{
    std::vector<std::string> targets = get_target(line);

    if (targets.empty())
        return; // ERR_NORECIPIENT

    for (size_t i = 0; i < targets.size(); ++i)
    {
        if (targets[i][0] == '#') // Es un canal
        {
            Channel *channel = server.get_channel(targets[i]);
            if (!channel)
                continue; // ERR_NOSUCHNICK o ERR_NOSUCHCHANNEL

            const std::vector<Client> &users = channel->get_users();
            for (size_t j = 0; j < users.size(); ++j)
            {
                if (users[j].get_FD() == sender.get_FD())
                    continue; // No enviarse a sí mismo

                std::string msg = ":" + sender.get_nickname() + " PRIVMSG " + targets[i] + " :" + line.get_trailing() + "\r\n";
                send(users[j].get_FD(), msg.c_str(), msg.size(), 0);
            }
        }
        else // Es un usuario individual
        {
            Client *target = server.get_client(targets[i]);
            if (!target)
                continue; // ERR_NOSUCHNICK

            std::string msg = ":" + sender.get_nickname() + " PRIVMSG " + targets[i] + " :" + line.get_trailing() + "\r\n";
            send(target->get_FD(), msg.c_str(), msg.size(), 0);
        }
    }
}

// /PRIVMSG #general,user1,user2,#echo
std::vector<std::string> get_target(RawTextLine &line)
{
	std::vector<std::string> targets;
	std::string token;

	std::stringstream ss(line.get_params()[0]);
	while (std::getline(ss, token, ','))
		targets.push_back(token);

	for (size_t i = 0; i < targets.size(); i++)
		std::cout << targets[i] << std::endl;

	return targets;
}