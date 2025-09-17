/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_who.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielrial <gabrielrial@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 11:13:06 by grial             #+#    #+#             */
/*   Updated: 2025/09/17 13:42:25 by gabrielrial      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib_irc.hpp"

//	Command: WHO
//	Parameters: [<name> [<o>]]

bool wildcard(RawTextLine &line);

void cmd_who(Server &server, RawTextLine &line, Client &client)
{
	if (line.get_params().empty())
	{
		std::cout << "Return an error cmd_who()" << std::endl;
		return;
	}
	else if (!server.check_channel(line))
	{
		std::cout << "Channel does not exist cmd_who()" << std::endl;
		return;
	}

	for (size_t i = 0; i < line.get_params().size(); i++)
	{
		std::string prefix = ":" +
							 std::string(SERVER_NAME) + " 332 " +
							 client.get_nickname() + " * " + line.get_params()[i] + " :";
		std::string name_list;
		for (unsigned int x = 0; x < server.get_channel(line.get_params()[0])->get_UserCount(); x++)
		{
			std::string name = server.get_vector_channels()[0].get_users()[x].get_nickname();
			if (std::string(prefix + name_list + name + " ").length() > BUFFER_LINE)
			{
				send(client.get_FD(), (prefix + name_list + CRLF).c_str(), (prefix + name_list + CRLF).length(), 0);
				name_list.clear();
				name_list = prefix + name;
			}
			else if (name_list.empty())
				name_list = name;
			else
				name_list = name_list + " " + name;
		}

		send(client.get_FD(), (prefix + name_list + CRLF).c_str(), (prefix + name_list + CRLF).length(), 0);
		std::cout << prefix + name_list + CRLF << std::endl;
		//: lead.libera.chat 366 TuNick #linux :End of /NAMES list.
		prefix = ":" + std::string(SERVER_NAME) + " 366 " + client.get_nickname() + " " + line.get_params()[i] + " :";
		send(client.get_FD(), (prefix + "End of /WHO list." + CRLF).c_str(), (prefix + "End of /WHO list." + CRLF).length(), 0);
		std::cout << prefix + "End of /WHO list." << std::endl;
	}
}

void cmd_who2(Server &server, RawTextLine &line, Client &client)
{
	bool only_op = only_operators(line.get_params());
	
}

bool wildcard(RawTextLine &line)
{
	if (strchr(line., '*'))
		return true;
	return false;
}

bool only_operators(const std::vector<std::string> &params)
{
    if (params.size() < 2)
        return false;

    return (params[1] == "o");
}

bool is_operator(RawTextLine &line)
{
}

std::vector<Client> *search_clients(Server &server, char *str)
{
	std::vector<Client> client_list;
	std::vector<Client> list = server.get_vector_clients();

	size_t size = list.size();
	for (size_t i = 0; i < size; i++)
	{
		if (strcmp(str, list[i].get_nickname))
			client_list = new Client(list[i])
	}
	return client_list;
}
