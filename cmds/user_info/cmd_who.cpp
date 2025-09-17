/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_who.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grial <grial@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 11:13:06 by grial             #+#    #+#             */
/*   Updated: 2025/09/17 17:49:04 by grial            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib_irc.hpp"

//	Command: WHO
//	Parameters: [<name> [<o>]]
//
// WHO → todos los visibles.
// WHO 0 → todos los visibles.
// WHO <#canal> → usuarios de ese canal.
// WHO <#canal> o → solo operadores en ese canal.
// WHO <mask> → usuarios cuyo nick/host/realname/server matchea el patrón.
// WHO <mask> o → solo operadores que cumplen el patrón.

bool is_valid_who(RawTextLine &line);
bool only_operators(const std::vector<std::string> &params);
std::string wildcard(RawTextLine &line);
std::vector<Client> mask_user(Server &server, bool only_op, const char *mask);
std::vector<Client> channel_users(bool only_op, const Channel *channel);
std::vector<Client> get_users(Server &server, bool only_op);
const Channel *get_channel(Server &server, RawTextLine &line);
std::vector<Client> get_list(Server &server, RawTextLine &line);

void cmd_who(Server &server, RawTextLine &line, Client &client)
{
	if (!is_valid_who(line))
		return; // TODO: enviar ERR_NOSUCHSERVER o mensaje de error

	std::vector<Client> client_list = get_list(server, line);

	std::string target = "*";
	if (line.get_sep_params().size() > 1 && line.get_sep_params()[1] != "o")
		target = line.get_sep_params()[1];

	for (size_t i = 0; i < client_list.size(); i++)
	{
		Client &c = client_list[i];

		std::string reply = ":" + std::string(SERVER_NAME) + " 352 " +
							client.get_nickname() + " " +
							target + " " +
							c.get_username() + " " +
							c.get_hostname() + " " +
							SERVER_NAME + " " +
							c.get_nickname() + " H";

		if (c.get_operator())
			reply += "*";

		reply += " :0 " + c.get_realname() + CRLF;

		send(client.get_FD(), reply.c_str(), reply.length(), 0);
	}

	std::string endmsg = ":" + std::string(SERVER_NAME) + " 315 " +
						 client.get_nickname() + " " + target + " :End of WHO list" + CRLF;

	send(client.get_FD(), endmsg.c_str(), endmsg.length(), 0);
}

bool is_valid_who(RawTextLine &line)
{
	std::vector<std::string> param = line.get_sep_params();

	if (param.size() > 3)
		return false; // demasiados parámetros
	if (param.size() == 3 && !(param[2].size() == 1 && param[2] == "o"))
		return false; // el tercer parámetro solo puede ser "o"
	return true;
}

std::vector<Client> get_list(Server &server, RawTextLine &line)
{
	std::vector<Client> client_list;

	std::string mask = wildcard(line);
	const Channel *channel = get_channel(server, line);
	bool only_op = only_operators(line.get_sep_params());

	if (line.get_sep_params().size() == 1 || 
	    (line.get_sep_params().size() == 2 && only_op)) // WHO, WHO o
		client_list = get_users(server, only_op);
	else if (line.get_sep_params().size() > 1 && channel) // WHO #channel [o]
		client_list = channel_users(only_op, channel);
	else if (!mask.empty()) // WHO mask [o]
		client_list = mask_user(server, only_op, mask.c_str());

	return client_list;
}

const Channel *get_channel(Server &server, RawTextLine &line)
{
	if (line.get_sep_params().size() < 2)
		return NULL;

	const char *param = line.get_sep_params()[1].c_str();

	if (!param || param[0] != '#')
		return NULL;

	const std::vector<Channel> &channels = server.get_vector_channels();
	size_t size = channels.size();

	for (size_t i = 0; i < size; i++)
	{
		if (strcmp(channels[i].get_name().c_str(), param) == 0)
			return &channels[i];
	}
	return NULL;
}

std::vector<Client> get_users(Server &server, bool only_op)
{
	std::vector<Client> clients = server.get_vector_clients();
	std::vector<Client> result;

	for (size_t i = 0; i < clients.size(); i++)
	{
		if (clients[i].get_visible() && (!only_op || clients[i].get_operator()))
			result.push_back(clients[i]);
	}
	return result;
}

std::vector<Client> channel_users(bool only_op, const Channel *channel)
{
	std::vector<Client> clients = channel->get_users();
	std::vector<Client> result;

	for (size_t i = 0; i < clients.size(); i++)
	{
		if (clients[i].get_visible() && (!only_op || clients[i].get_operator()))
			result.push_back(clients[i]);
	}
	return result;
}

std::vector<Client> mask_user(Server &server, bool only_op, const char *mask)
{
	std::vector<Client> result;
	std::vector<Client> clients = server.get_vector_clients();

	for (size_t i = 0; i < clients.size(); i++)
	{
		bool match =
			strcmp(clients[i].get_nickname().c_str(), mask) == 0 ||
			strcmp(clients[i].get_username().c_str(), mask) == 0 ||
			strcmp(clients[i].get_hostname().c_str(), mask) == 0 ||
			strcmp(clients[i].get_realname().c_str(), mask) == 0;

		if (match && clients[i].get_visible() && (!only_op || clients[i].get_operator()))
			result.push_back(clients[i]);
	}
	return result;
}

std::string wildcard(RawTextLine &line)
{
	if (line.get_sep_params().size() < 2)
		return "";

	std::string param = line.get_sep_params()[1];

	size_t wpos = param.find('*');
	if (wpos != std::string::npos)
	{
		if (wpos == 0)
			return param.substr(1);
		else
			return param.substr(0, wpos);
	}
	return "";
}

bool only_operators(const std::vector<std::string> &params)
{
	if (params.empty())
		return false;

	return params.back() == "o";
}
