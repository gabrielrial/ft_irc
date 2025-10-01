#include "../bot_cmds_list.hpp"
#include "../../Bot.hpp"

void bot_privmsg(RawTextLine &line, Bot &bot)
{
    std::cout << "trailing: " << line.get_trailing() << std::endl;

    if (line.get_trailing() == "!joke")
        cmd_joke(line, bot);
    else if (line.get_trailing() == "!time")
        cmd_time(line, bot);
    else if (line.get_trailing() == "!filter")
        bot_filter(line, bot);
    else if (line.get_trailing() == "!help")
    {
        for (size_t i = 0; i < line.get_params().size(); i++)
        {
            std::string client = line.get_params()[i];
            if (line.get_params()[i] == bot.get_nickname())
                client = bot.get_client_nickname(line);
            bot.send_message(bot.get_socket(),
                "PRIVMSG " + client + " :I only work with !joke and !time commands. You can also invite me to channels.");
        }
    }

    if (bot.get_filter())
    {
        std::cout << "kicking1" << std::endl;
        if (line.get_trailing().find("fuck") != std::string::npos)
        {
            std::cout << "kicking" << std::endl;
            bot.send_message(bot.get_socket(),
                "KICK " + line.get_params()[0] + " " +
                bot.get_client_nickname(line) + " :" +
                bot.get_client_nickname(line) + " has been kicked for bad word");
        }
    }
}
