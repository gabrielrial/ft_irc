#include "../../lib_irc.hpp"

void cmd_name(Server &server, RawTextLine &line, Client &client)
{
	if (line.get_params().empty())
		return; // no channel

	Channel *channel = server.get_channel(line.get_params()[0]);
	if (!channel)
	{
		// channel does not exist -> send RPL_ENDOFNAMES
		std::string endmsg = ":" + server.get_servername() + " 366 " +
							 client.get_nickname() + " " + line.get_params()[0] +
							 " :End of NAMES list" + CRLF;
		send(client.get_FD(), endmsg.c_str(), endmsg.size(), 0);
		return;
	}

	std::cout << line.get_params()[0] << std::endl;
	std::vector<Client> client_list = channel->get_users();

	std::string rpl_namreply = ":" + server.get_servername() + " 353 " +
							   client.get_nickname() + " = " + channel->get_name() + " :";

	std::string names_list;

	for (size_t i = 0; i < client_list.size(); i++)
	{
		std::string client_name = client_list[i].get_nickname();

		if ((rpl_namreply + names_list + client_name).size() < 510)
		{
			if (!names_list.empty())
				names_list += " ";
			names_list += client_name;
		}
		else
		{
			std::string msg = rpl_namreply + names_list + CRLF;
			for (size_t x = 0; i < channel->get_users().size(); x++)
			{
				send(channel->get_users()[x].get_FD(), msg.c_str(), msg.size(), 0);
				std::string endmsg = ":" + server.get_servername() + " 366 " +
									 client.get_nickname() + " " + channel->get_name() +
									 " :End of NAMES list" + CRLF;
				send(channel->get_users()[x].get_FD(), endmsg.c_str(), endmsg.size(), 0);
			}
			names_list = client_name;
		}
	}

	if (!names_list.empty())
	{
		std::string msg = rpl_namreply + names_list + CRLF;
		send(client.get_FD(), msg.c_str(), msg.size(), 0);
	}
}
