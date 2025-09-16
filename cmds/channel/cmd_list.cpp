#include "../../lib_irc.hpp"

void cmd_list(Server &server, RawTextLine &line, Client &client)
{
    const std::vector<Channel> &channels = server.get_vector_channels();

    for (size_t i = 0; i < channels.size(); i++)
    {
        std::stringstream user_count_ss;
        user_count_ss << channels[i].get_UserCount();

        std::string topic = channels[i].get_topic();
        if (topic.empty())
            topic = "No topic set";

        std::string channel_info = ":" +
            std::string(SERVER_NAME) + " 322 " +
            client.get_nickname() + " " +
            channels[i].get_name() + " " +
            user_count_ss.str() + " :" +
            topic + "\r\n";

        send(client.get_FD(), channel_info.c_str(), channel_info.length(), 0);
        std::cout << "Sent LIST info: " << channel_info;
    }

    std::string channel_info = ":" +
        std::string(SERVER_NAME) + " 323 " +
        client.get_nickname() + " :End of /LIST\r\n";

    send(client.get_FD(), channel_info.c_str(), channel_info.length(), 0);
    std::cout << "Sent LIST info: " << channel_info;

    (void)line;
}
