#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <poll.h>
#include <unistd.h>

class Client {
private:
    int _socket;
    std::string _name;
    std::string _hostname;
    pollfd _pollClient;

public:
    Client(int socket);
    ~Client();

    int getSocket() const;
    void setName(const std::string &name);
    std::string getName() const;
    pollfd* getPollFD();
};

#endif
