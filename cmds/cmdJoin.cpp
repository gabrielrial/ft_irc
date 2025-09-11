#include "cmds_list.hpp"

int cmdJoin(RawTextLine msg, Client cinfo)
{
    // Check if we have any parameters
    const std::vector<std::string>& params = msg.getParams();
    if (params.empty())
    {
        // ERR_NEEDMOREPARAMS (461)
        std::string error = ":localhost 461 " + cinfo.getNickname() + " JOIN :Not enough parameters\r\n";
        send(cinfo.getFd(), error.c_str(), error.length(), 0);
        return 1;
    }

    // First parameter contains comma-separated channel list
    std::vector<std::string> channels = msg.getSepParams();
    
    // Check each channel name
    for (std::vector<std::string>::const_iterator it = channels.begin(); it != channels.end(); ++it)
    {
        std::string channelName = *it;
        
        // Add # if not present
        if (channelName[0] != '#')
            channelName = "#" + channelName;

        // Create or get existing channel
        Channel* channel = new Channel(channelName);  // You'll need channel management in your Server class
        
        // Add user to channel (this will need to be implemented in Channel class)
        // channel->addUser(cinfo);
        
        // Send JOIN confirmation to the channel
        std::string joinMsg = ":" + cinfo.getNickname() + "!" + cinfo.getUsername() + "@" + 
                            cinfo.getIP() + " JOIN " + channelName + "\r\n";
        
        // Send to all users in channel (this will need to be implemented in Channel class)
        channel->broadcast(joinMsg);
        
        // Send channel topic if it exists
        std::string topic = channel->getTopic();
        if (!topic.empty())
        {
            std::string topicMsg = ":localhost 332 " + cinfo.getNickname() + " " + 
                                channelName + " :" + topic + "\r\n";
            send(cinfo.getFd(), topicMsg.c_str(), topicMsg.length(), 0);
        }
        
        // TODO: Send names list (353) and end of names (366)
        // This requires implementation of getting user list from channel
    }
    
    return 0;
}