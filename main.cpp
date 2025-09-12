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
    (void) argv;
    if (argc != 3)
    {
        std::cerr << "Please use the following format:" << std::endl;
        std::cerr << "./ircserv <port> <password>" << std::endl;
        return 1;
    }
    uint16_t port = static_cast<uint16_t>(std::atoi(argv[1]));
    std::string password = argv[2];
    Server srv(port, password);
    srv.srv_run();
    return 0;
}