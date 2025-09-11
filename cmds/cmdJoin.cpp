#include "../lib_irc.hpp"

void	cmdJoin(Server &server, RawTextLine &line, Client &client)
{
	if (checkParams(line, client) == 1)
		return ;
	std::vector<std::string> channels = line.getSepParams();
	for (std::vector<std::string>::const_iterator it = channels.begin(); it != channels.end(); ++it)
	{
		std::string channelName = *it;
		if (channelName[0] != '#')
			channelName = "#" + channelName;
		Channel* channel = new Channel(channelName);
		
		// Add user to channel (this will need to be implemented in Channel class)
		// channel->addUser(client);
		// Send JOIN confirmation to the channel
		std::string joinline = ":" + client.getNickname() + "!" + client.getUsername() + "@" + 
							client.getIP() + " JOIN " + channelName + "\r\n";
		
		// Send to all users in channel (this will need to be implemented in Channel class)
		channel->broadcast(joinline);
		
		// Send channel topic if it exists
		std::string topic = channel->getTopic();
		if (!topic.empty())
		{
			std::string topicline = ":localhost 332 " + client.getNickname() + " " + 
								channelName + " :" + topic + "\r\n";
			send(client.getFd(), topicline.c_str(), topicline.length(), 0);
		}
		
		// TODO: Send names list (353) and end of names (366)
		// This requires implementation of getting user list from channel
	}
	
	return;
}

int	checkParams(RawTextLine &line, Client &client)
{
	const std::vector<std::string>& params = line.getParams();
	if (params.empty())
	{
		std::string error = ":localhost 461 " + client.getNickname() + " JOIN :Not enough parameters\r\n";
		send(client.getFd(), error.c_str(), error.length(), 0);
		return 1;
	}
	return 0;
}