#include "../../lib_irc.hpp"

void cmd_list(Server &server, RawTextLine &line, Client &client)
{
    const std::vector<Channel> &channels = server.get_vector_channels();

    for (size_t i = 0; i < channels.size(); i++)
    {
        std::stringstream user_count_ss;
        user_count_ss << channels[i].get_UserCount();

        std::string topic = channels[i].getTopic();
        if (topic.empty())
            topic = "No topic set";

        std::string channel_info = ":" +
            std::string(SERVER_NAME) + " 322 " +
            client.getNickname() + " " +
            channels[i].getName() + " " +
            user_count_ss.str() + " :" +
            topic + "\r\n";

        send(client.getFd(), channel_info.c_str(), channel_info.length(), 0);
        std::cout << "Sent LIST info: " << channel_info;
    }

    std::string channel_info = ":" +
        std::string(SERVER_NAME) + " 323 " +
        client.getNickname() + " :End of /LIST\r\n";

    send(client.getFd(), channel_info.c_str(), channel_info.length(), 0);
    std::cout << "Sent LIST info: " << channel_info;

    (void)line;
}
