#include "ChannelManager.hpp"

ChannelManager::ChannelManager()
{}

ChannelManager::~ChannelManager()
{
	// Clean up all channels
	for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		delete it->second;
	}
	_channels.clear();
}

Channel* ChannelManager::createChannel(const std::string& name)
{
	if (channelExists(name))
		return NULL;
	
	Channel* channel = new Channel(name);
	_channels[name] = channel;
	return channel;
}

Channel* ChannelManager::get_channel(const std::string& name)
{
	std::map<std::string, Channel*>::iterator it = _channels.find(name);
	if (it == _channels.end())
		return NULL;
	return it->second;
}

bool ChannelManager::deleteChannel(const std::string& name)
{
	std::map<std::string, Channel*>::iterator it = _channels.find(name);
	if (it == _channels.end())
		return false;
	
	delete it->second;
	_channels.erase(it);
	return true;
}

bool ChannelManager::channelExists(const std::string& name) const
{
	return _channels.find(name) != _channels.end();
}

// void ChannelManager::remove_userFromAllChannels(int client_fd)
// {
// 	for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
// 	{
// 		it->second->remove_user(client_fd);
// 	}
// }

const std::map<std::string, Channel*>& ChannelManager::get_channels() const
{
	return _channels;
}
