#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "lib_irc.hpp"
#include <netinet/in.h>
#include <string>
//#include "Channel.hpp"

class Channel;

class Client
{
    private:
        int _socket_fd;
        int _port;
        int _channel_amount; // needed? - limit to 10
        struct sockaddr_in _server_addr;
        std::string _ip;
        std::string _nickname;
        std::string _username;
        std::vector<Channel*> channels;
 //?       std::string realname; // add in
 //     hostname

    public:
        Client(int socket_fd, sockaddr_in &addr);
        ~Client();
        int get_channel_amt();
        int getFd() const;
        int getPort() const;
        std::string getIP() const;
        std::string getNickname() const;
        std::string getUsername() const;
        void setNickname(const std::string &nick);
        void setUsername(const std::string &user);
};

//bool add_to_channel(Client *client, Channel channel);

#endif