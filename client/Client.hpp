#pragma once

#include "../lib_irc.hpp"
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
        bool        _has_pass;
        bool        _has_nick;
        bool        _has_user;
        bool        _is_visible;
        bool        _is_operator;
        std::vector<Channel*> channels;

    public:
        Client(int client_fd, sockaddr_in &addr);
        ~Client();

        int get_FD() const;
        std::string get_IP() const;
        std::string get_nickname() const;
        std::string get_username() const;
        std::string get_realname() const;
        std::string get_hostname() const;
        std::string get_servername() const;
        void pass_accepted(bool accepted);
        void set_nickname(const std::string &nick);
        void set_username(const std::string &user);
        void set_realname(const std::string &real);
        void set_hostname(const std::string &host);
        void set_operator();
        void set_visible();
        void set_servername(const std::string &server);
        bool has_pass() const;
        bool has_nick() const;
        bool has_user() const;
        bool get_visible() const;
        bool get_operator() const;
        bool is_registered();
        int get_channel_amt();
		bool operator==(const Client& other) const;
};

