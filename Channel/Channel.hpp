#pragma once
#include "../Client/Client.hpp"
#include "../Server/Server.hpp"
#include <string>
#include <map>
#include <vector>

class Channel
{
private:
    std::string             name;
    std::string             topic;
    std::string             password;
    std::map<int, Client *>  users;
    std::map<int, Client *>  operators;  
    std::vector<int>        invit_list;
    int                     members_limit;

    bool        invit_only;   // +i
    bool        topic_ch_restric; // +t
    bool        has_pswd;  // +k
    bool        limited; // +l

public:
    Channel(const std::string &name);
    ~Channel();

    const std::string &get_name() const;
    const std::string &get_topic() const;
    const std::string &get_password() const;

    const std::map<int, Client *> &get_users() const;
    size_t get_nbr_of_users() const;
    int get_members_limit() const;
    const std::map<int, Client *> &get_operators() const;
    const std::vector<int> &get_invit_list() const;
    
    bool get_invit_only();
    bool get_topic_ch_restrict();
    bool get_has_pswd();
    bool get_limited();

    void set_invit_only(bool value);
    void set_topic_ch_restric(bool value);
    void set_has_pswd(bool value);
    void set_limited(bool value);

    void set_topic(const std::string &topic);
    void set_password(const std::string &password);
    void remove_password();
    void set_members_limit(const int members_limit);
    void remove_members_limit();



    void add_user(Client &);
    void remove_user(int fd);
    bool is_user(int fd) const;

    void add_op(Client &);
    void remove_op(int fd);
    bool is_op(int fd) const;

    void add_invit(int fd);
    void remove_invit(int fd);
    int is_invit(int fd) const; // return index of the fd in invit_list

    void broadcast(const std::string &message, int except_fd);
};
