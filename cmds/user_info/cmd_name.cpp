#include "../../lib_irc.hpp"

void cmd_name(Server &server, RawTextLine &line, Client &client)
{
	if (!is_valid_name(server, line, client))
		return;

	if (line.get_params().empty())
	{
		std::cout << "!@# run cmd_name() without parameters" << std::endl;
	}

	Channel *channel = server.get_channel(line.get_params()[0]);
	if (!channel)
	{
		err_nosuchchannel(server.get_servername(), client, line.get_params()[0]);
		return;
	}

	if (channel->get_mode_i() || (!channel->get_mode_k().empty() && !channel->check_user(client.get_nickname())))
	{
		std::cout << "!@# check if I'm invited" << std::endl;
		std::cout << "!@# run cmd_name() without parameters" << std::endl;
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

bool is_valid_name(Server &server, RawTextLine &line, Client &client)
{
	if (line.get_params().size() > 1)
	{
		std::cout << "rerror" << std::endl;
		return false;
	}
	else if (!line.get_trailing().empty())
	{
		std::cout << "rerror" << std::endl;
		return false;
	}
	return true;
}
