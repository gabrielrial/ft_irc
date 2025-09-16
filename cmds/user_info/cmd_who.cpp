/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_who.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grial <grial@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 11:13:06 by grial             #+#    #+#             */
/*   Updated: 2025/09/16 16:43:38 by grial            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib_irc.hpp"

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
		//:lead.libera.chat 366 TuNick #linux :End of /NAMES list.
		prefix = ":" + std::string(SERVER_NAME) + " 366 " + client.get_nickname() + " " + line.get_params()[i] + " :";
		send(client.get_FD(), (prefix + "End of /WHO list." + CRLF).c_str(), (prefix + "End of /WHO list." + CRLF).length(), 0);
		std::cout << prefix + "End of /WHO list." << std::endl;

	}
}