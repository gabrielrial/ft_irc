#pragma once

#include "../lib_irc.hpp"

class Bot : public Client
{
public:
    Bot();
    Bot(const Bot &other);
    Bot &operator=(const Bot &other);
    ~Bot();

std::string _name

    private:
};
std::ostream &operator<<(std::ostream &os, const Bot &Bot);