#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "lib_irc.hpp"
#include <netinet/in.h>
#include <string>
#include <vector>

class Channel;

class Client
{
    private:
        int         _client_fd;
        int         _channel_amount;
        std::string _ip;
        std::string _nickname;
        std::string _username;
        std::string _realname;
        std::string _hostname;
        std::string _servername;
        bool        _has_nick;
        bool        _has_user;
        std::vector<Channel*> channels;

    public:
        Client(int client_fd, sockaddr_in &addr);
        ~Client();

        int getFd() const;
        std::string getIP() const;
        std::string getNickname() const;
        std::string getUsername() const;
        std::string getRealname() const;
        std::string getHostname() const;
        std::string getServername() const;
        void setNickname(const std::string &nick);
        void setUsername(const std::string &user);
        void setRealname(const std::string &real);
        void setHostname(const std::string &user);
        void setServername(const std::string &real);
        bool isRegistered();
        int get_channel_amt();
};

#endif