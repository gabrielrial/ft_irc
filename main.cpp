#include "lib_irc.hpp"

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