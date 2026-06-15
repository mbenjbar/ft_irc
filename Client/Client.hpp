#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <sys/socket.h>

class Client
{
private:
    int _fd;
    std::string nickname;
    std::string username;
    std::string realname;
    std::string buffer;
    bool registered;
    bool passed;
    bool has_nick;
    bool has_user;

public:
    Client(int fd);
    ~Client();


    int getFd() const;
    const std::string &get_Nickname() const;
    const std::string &get_Username() const;
    const std::string &get_Realname() const;
    std::string &get_Buffer();
    bool is_Registered() const;
    bool is_passed() const;
    bool Nick_named() const;
    bool hasUser() const;

    void set_Nickname(const std::string& nick);
    void set_Username(const std::string& user);
    void set_Realname(const std::string& real);
    void set_Pass(bool value);
    void set_Has_nick(bool value);
    void set_Has_user(bool value);
    void set_Registered(bool value);

    void append_buff(const std::string &received);
    void clear_buff();

    void send_msg(const std::string &msg) const;

    std::string make_prefix() const;
};