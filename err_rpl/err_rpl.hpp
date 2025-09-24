#pragma once
# include "../lib_irc.hpp"

//err.cpp
void	err_needmoreparams(std::string server_name, Client &client, std::string cmd);
void	err_nosuchchannel(std::string server_name, Client &client, std::string channel_name);
void	err_notonchannel(std::string server_name, Client &client, Channel *channel);
void	err_chanoprivsneeded(std::string server_name, Client &client, Channel *channel);
void	err_nosuchnick(std::string server_name, Client &client, std::string nickname, std::string cmd);
void	err_unknownmode(std::string server_name, Client &client, char mode);
void	err_usernotinchannel(std::string server_name, Client &client, Channel *channel, std::string nickname);
void	err_badchannelkey(std::string server_name, Client &client, Channel *channel);
void	err_channelisfull(std::string server_name, Client &client, Channel *channel);

//rpl.cpp
void	rpl_notopic(std::string server_name, Client &client, std::string channel_name);
void	rpl_topic(std::string server_name, Client &client, std::string channel_name, std::string topic);
void	rpl_namreply(const std::string server_name, const Client &client, const std::string channel_name, const std::string user_list);
void	rpl_endofnames(const std::string server_name, const Client &client, const std::string channel_name);
void	rpl_channelmodeis(std::string server_name, const Client &client, std::string channel_name, std::string modestring);