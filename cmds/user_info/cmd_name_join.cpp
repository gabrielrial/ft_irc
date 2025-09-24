#include "../../lib_irc.hpp"

void cmd_name_join(Server &server, std::string &channel_name)
{
	Channel *channel = server.get_channel(channel_name);
		std::cout << "hola " << channel->get_name() << std::endl; 
	std::vector<Client> client_list = channel->get_users();

	for (size_t c = 0; c < client_list.size(); c++)
	{
		Client &target = client_list[c];
		std::string names_list;

		for (size_t i = 0; i < client_list.size(); i++)
		{
			std::string rpl_namreply = ":" + server.get_servername() + " 353 " +
									   target.get_nickname() + " = " + channel->get_name() + " :";

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
				send(target.get_FD(), msg.c_str(), msg.size(), 0);
				names_list = client_name;
			}
			if (i == client_list.size() - 1 && !names_list.empty())
			{
				std::string msg = rpl_namreply + names_list + CRLF;
				send(target.get_FD(), msg.c_str(), msg.size(), 0);
			}
		}
		std::string endmsg = ":" + server.get_servername() + " 366 " +
							 target.get_nickname() + " " + channel->get_name() +
							 " :End of NAMES list" + CRLF;
		send(target.get_FD(), endmsg.c_str(), endmsg.size(), 0);
	}
}
