#pragma once
# include "../lib_irc.hpp"

std::string		get_servername();
void			err_needmoreparams(std::string server_name, Client &client, std::string cmd);
void			err_nosuchchannel(std::string server_name, Client &client, Channel *channel);
void			err_notonchannel(std::string server_name, Client &client, Channel *channel);
void			err_chanoprivsneeded(std::string server_name, Client &client, Channel *channel);
void			err_nosuchnick(std::string server_name, Client &client, std::string nickname, std::string cmd);
void			err_unknownmode(std::string server_name, Client &client, char mode);

