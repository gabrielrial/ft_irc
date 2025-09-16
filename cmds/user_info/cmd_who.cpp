/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_who.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grial <grial@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 11:13:06 by grial             #+#    #+#             */
/*   Updated: 2025/09/16 13:52:27 by grial            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib_irc.hpp"

void cmd_who(Server &server, RawTextLine &line, Client &client)
{
	if (line.getParams().empty())
	{
		std::cout << "Return an error cmd_who()" << std::endl;
		return;
	}
	else if (!server.check_channel(line))
	{
		std::cout << "Channel does not exist cmd_who()" << std::endl;
		return;
	}
	for (size_t i = 0; i < server.get_vector_channels().size(); i++)
	{
		server.get_vector_channels()[i].getUsers();
	}
}