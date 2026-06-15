#include "Channel.hpp"

Channel::Channel(const std::string &name)
{
    this->name = name;
    topic = "";
    password = "";
    invit_only = false;
    topic_ch_restric = false;
    has_pswd = false;
    limited = false;
    members_limit = 0;
}
Channel::~Channel(){}

const std::string &Channel::get_name() const {return name;}
const std::string &Channel::get_topic() const {return topic;}
const std::string &Channel::get_password() const {return password;}

const std::map<int, Client *>& Channel::get_users() const {return users;}
size_t Channel::get_nbr_of_users() const {return users.size();}
const int Channel::get_members_limit() const {return members_limit;}
const std::map<int, Client *> &Channel::get_operators() const {return operators;}
const std::vector<int> &Channel::get_invit_list() const {return invit_list;}
    
bool Channel::get_invit_only() {return invit_only;}
bool Channel::get_topic_ch_restrict() {return topic_ch_restric;}
bool Channel::get_has_pswd() {return has_pswd;}
bool Channel::get_limited() {return limited;}

void Channel::set_invit_only(bool value) {invit_only = value;}
void Channel::set_topic_ch_restric(bool value) {topic_ch_restric = value;}
void Channel::set_has_pswd(bool value) {has_pswd = value;}
void Channel::set_limited(bool value) {limited = value;}

void Channel::set_topic(const std::string &topic) {this->topic = topic;}
void Channel::set_password(const std::string &password) {this->password = password; set_has_pswd(true);}
void Channel::set_members_limit(const int members_limit) {this->members_limit = members_limit; set_limited(true);}
void Channel::remove_members_limit() {this->members_limit = 0; set_limited(false);}



void Channel::add_user(Client &client) {users[client.getFd()] = &client;}
void Channel::remove_user(int fd) {users.erase(fd); operators.erase(fd);}
bool Channel::is_user(int fd) const {return users.find(fd) != users.end() ? true : false;}

void Channel::add_op(Client &client) {operators[client.getFd()] = &client;}
void Channel::remove_op(int fd) {operators.erase(fd);}
bool Channel::is_op(int fd) const {return operators.find(fd) != operators.end() ? true : false;}

void Channel::add_invit(int fd) {if (is_invit(fd) == -1) invit_list.push_back(fd);}
void Channel::remove_invit(int fd) 
{
    int i = is_invit(fd);
    if (i >= 0) invit_list.erase(invit_list.begin() + i);
}
int Channel::is_invit(int fd) const
{
    int i = 0;
    while (i++ < invit_list.size())
    {
        if (invit_list[i] == fd)
            return i;
    }
    return -1;
}

void Channel::broadcast(const std::string &message, int except_fd)
{    
    std::map<int, Client*>::iterator it;
    for (it = users.begin(); it != users.end(); it++)
    {
        if (it->first != except_fd)
            send(it->first, message.c_str(), message.size(), 0);
    }
}