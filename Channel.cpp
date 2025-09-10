#include "Channel.hpp"
#include "Client.hpp"

Channel::Channel()
{
    this->name = "#empty_channel";
}

Channel::Channel(std::string name)
{
    this->name = name;
}
