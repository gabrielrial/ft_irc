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
        int _client_fd;
        int _channel_amount;
        std::string _ip;
        std::string _nickname;
        std::string _username;
        std::string _realname;
        std::vector<Channel*> channels;
 //     hostname

    public:
        Client(int socket_fd, sockaddr_in &addr);
        ~Client();
        int get_channel_amt();
        int getFd() const;
        std::string getIP() const;
        std::string getNickname() const;
        std::string getUsername() const;
        std::string getRealname() const;
        void setNickname(const std::string &nick);
        void setUsername(const std::string &user);
};

//bool add_to_channel(Client *client, Channel channel);

#endif