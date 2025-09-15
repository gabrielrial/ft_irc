#pragma once

#include "Channel.hpp"
#include <map>

class ChannelManager
{
private:
	std::map<std::string, Channel*> _channels;

public:
	ChannelManager();
	~ChannelManager();

	// Channel management
	Channel*	createChannel(const std::string& name);
	Channel*	get_channel(const std::string& name);
	bool		deleteChannel(const std::string& name);
	bool		channelExists(const std::string& name) const;
	
	// User management across channels
	void		remove_userFromAllChannels(int client_fd);
	
	// Getters
	const std::map<std::string, Channel*>& get_channels() const;
};
