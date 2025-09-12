#include "lib_irc.hpp"

int	invalid_arguments(char **argv)
{
    if (std::string(argv[1]) != "6667")
        return 1;
    if (std::string(argv[2]) != "password42")
        return 1;
    return 0;
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Please use the following format:" << std::endl;
        std::cerr << "./ircserv <port> <password>" << std::endl;
        return 1;
    }
    if (invalid_arguments(argv))
    {
        std::cout << "Incorrect port number or password entered. Please try again." << std::endl;
        return 0;
    }
    Server srv;
    srv.srv_run();
    return 0;
}