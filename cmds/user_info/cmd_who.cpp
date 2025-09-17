/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_who.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grial <grial@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 11:13:06 by grial             #+#    #+#             */
/*   Updated: 2025/09/17 17:11:01 by grial            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib_irc.hpp"

//	Command: WHO
//	Parameters: [<name> [<o>]]

// WHO → todos los visibles.
//
// WHO 0 → todos los visibles.
//
// WHO <#canal> → usuarios de ese canal.
//
// WHO <#canal> o → solo operadores en ese canal.
//
// WHO <mask> → usuarios cuyo nick/host/realname/server matchea el patrón.
//
// WHO <mask> o → solo operadores que cumplen el patrón.

std::string wildcard(RawTextLine &line);

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
	if (!is_valid_who(line))
		return; // report error

	std::vector<Client> client_list;

	client_list = get_list(server, line);

	std::string prefix = ":" +
						 std::string(SERVER_NAME) + " 352 " +
						 client.get_nickname() + " * " + line.get_params()[] + " :";
	std::string name_list;
	for (size_t i = 0; i < client_list.size(); i++)
	{
		if (i > 0)
		{
			name_list = name_list + " " + client_list[i].get_nickname();
		}
		else
			name_list = client_list[i].get_nickname();
	}
	send(client.get_FD(), (prefix + name_list + CRLF).c_str(), (prefix + name_list + CRLF).length(), 0);
	std::cout << prefix + name_list + CRLF << std::endl;
	prefix = ":" + std::string(SERVER_NAME) + " 315 " + client.get_nickname() + " " + line.get_params()[i] + " :";
	send(client.get_FD(), (prefix + "End of /WHO list." + CRLF).c_str(), (prefix + "End of /WHO list." + CRLF).length(), 0);
}

bool is_valid_who(RawTextLine &line)
{
	std::vector<std::string> param = line.get_sep_params();

	if (param.size() > 3)
		return false; // report error
	if (param.size() == 3 && param[2].size() != 1 && param[2] != "o")
		return false; // report error
	return true;
}

std::vector<Client> get_list(Server &server, RawTextLine &line)
{
	std::vector<Client> client_list;

	std::string mask = wildcard(line);
	Channel *channel = get_channel(server, line);
	bool only_op = only_operators(line.get_params());

	if (line.get_sep_params().size() == 1 || (line.get_sep_params().size() == 2 && only_op)) // WHO, WHO o
		client_list = get_users(server, only_op);
	else if (line.get_sep_params().size() > 1 && channel) // WHO channel [o]
		client_list = channel_users(server, only_op, channel);
	else if (!mask.empty())
		client_list = mask_user(server, only_op, mask.c_str()); // WHO mask [o]

	return client_list;
}

Channel *get_channel(Server &server, RawTextLine &line)
{
	if (line.get_sep_params().size() < 1)
		return NULL;

	const char *param = line.get_sep_params()[1].c_str();

	if (param && param[0] != '#')
		return NULL;

	std::vector<Channel> channel = server.get_vector_channels();
	size_t size = channel.size();

	for (size_t i = 0; i < size; i++)
	{
		if (strcmp(channel[i].get_name().c_str(), param))
			return &channel[i];
	}
	return NULL; // report error
}

std::vector<Client> get_users(Server &server, bool only_op)
{
	std::vector<Client> clients = server.get_vector_clients();
	std::vector<Client> result;

	size_t size = clients.size();
	for (size_t i = 0; i < size; i++)
	{
		if (clients[i].get_visible() && !only_op)
			result.push_back(clients[i]);
		else if (clients[i].get_visible() && only_op && clients[i].get_operator())
			result.push_back(clients[i]);
	}
	return (result);
}

std::vector<Client> channel_users(Server &server, bool only_op, Channel *channel)
{
	std::vector<Client> clients = channel->get_users();
	std::vector<Client> result;

	for (size_t i = 0; i < clients.size(); i++)
	{
		if (clients[i].get_visible() && !only_op)
			result.push_back(clients[i]);
		else if (clients[i].get_visible() && only_op && clients[i].get_operator())
			result.push_back(clients[i]);
	}
	return (result);
}

std::vector<Client> mask_user(Server &server, bool only_op, const char *mask)
{
	std::vector<Client> result;
	std::vector<Client> clients = server.get_vector_clients();
	size_t size = clients.size();

	for (size_t i = 0; i < size; i++)
	{
		if (strcmp(clients[i].get_nickname().c_str(), mask) || strcmp(clients[i].get_username().c_str(), mask) || strcmp(clients[i].get_hostname().c_str(), mask) || strcmp(clients[i].get_realname().c_str(), mask))
			result.push_back(clients[i]);
	}
	return clients;
}

std::string wildcard(RawTextLine &line)
{
	if (line.get_sep_params().size() < 2)
		return NULL;

	std::vector<std::string> param = line.get_sep_params()[1];

	if (strchr((param[1]).c_str(), '*'))
	{
		size_t wpos = param[1].find('*');
		if (wpos == 0)
			return (param[1].substr(1));
		else
			return (param[1].substr(0, wpos));
	}
	return NULL;
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
