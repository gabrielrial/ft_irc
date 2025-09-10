#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <string>
#include "Client.hpp"

class Channel
{
    private:
        std::string name;
        std::vector<Client*> clients;

    public:
        Channel();
        Channel(std::string name);
};

#endif