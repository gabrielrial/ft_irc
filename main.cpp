#include "lib_irc.hpp"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "three arguments" << std::endl;
        return 1;
    }
    
    Server srv;

    srv.srv_run();

    (void)argv;
    
}