#pragma once
# include "../lib_irc.hpp"

//err.cpp
void	err_nosuchnick(std::string server_name, Client &client, std::string nickname, std::string cmd);	// 401
void	err_nosuchchannel(std::string server_name, Client &client, std::string channel_name);			// 403
void	err_norecipient(std::string server_name, Client &client, std::string cmd);						// 411
void	err_notexttosend(std::string server_name, Client &client);										// 412
void	err_nonicknamegiven(std::string server_name, Client &client);									// 431
void	err_erroneusnickname(std::string server_name, Client &client, std::string newNick);				// 432
void	err_nicknameinuse(std::string server_name, Client &client, std::string attempt);				// 433
void	err_usernotinchannel(std::string server_name, Client &client, Channel *channel, std::string nickname);	// 441
void	err_notonchannel(std::string server_name, Client &client, Channel *channel);					// 442
void	err_useronchannel(std::string server_name, Client &client, std::string target_cli, std::string channel_name);	// 443
void	err_needmoreparams(std::string server_name, Client &client, std::string cmd);					// 461
void	err_alreadyregistered(std::string server_name, Client &client);									// 462
void	err_passwdmismatch(std::string server_name, Client &client);									// 464
void	err_channelisfull(std::string server_name, Client &client, Channel *channel);					// 471
void	err_unknownmode(std::string server_name, Client &client, char mode);							// 472
void	err_badchannelkey(std::string server_name, Client &client, Channel *channel);					// 475
void	err_badchanmask(std::string server_name, Client &client, std::string channel_name);				// 476
void	err_chanoprivsneeded(std::string server_name, Client &client, Channel *channel);				// 482

//rpl.cpp
void	rpl_notopic(std::string server_name, Client &client, std::string channel_name);
void	rpl_topic(std::string server_name, Client &client, std::string channel_name, std::string topic);
void	rpl_namreply(const std::string server_name, const Client &client, const std::string channel_name, const std::string user_list);
void	rpl_endofnames(const std::string server_name, const Client &client, const std::string channel_name);
void	rpl_channelmodeis(std::string server_name, const Client &client, std::string channel_name, std::string modestring);