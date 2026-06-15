#include "Client.hpp"

Client::Client(int fd)
{
    _fd = fd;
    passed = false;
    has_nick = false;
    has_user = false;
    registered = false;
    nickname = '*';
    username = "";
    realname = "";
    buffer = "";
}

Client::~Client() {}

int Client::getFd() const {return _fd;}
const std::string &Client::get_Nickname() const {return nickname;}
const std::string &Client::get_Username() const {return username;}
const std::string &Client::get_Realname() const {return realname;}
std::string &Client::get_Buffer() {return buffer;}
bool Client::is_Registered() const {return registered;}
bool Client::is_passed() const {return passed;}
bool Client::Nick_named() const {return has_nick;}
bool Client::hasUser() const {return has_user;}

void Client::set_Nickname(const std::string& nick) {nickname = nick; has_nick = true;}
void Client::set_Username(const std::string& user) {username = user; has_user = true;}
void Client::set_Realname(const std::string& real) {realname = real;}
void Client::set_Pass(bool value) {passed = value;}
void Client::set_Has_nick(bool value) {has_nick = value;}
void Client::set_Has_user(bool value) {has_user = value;}
void Client::set_Registered(bool value) {registered = value;}


void Client::append_buff(const std::string &received) {buffer = buffer + received;}
void Client::clear_buff() {buffer.clear();}
void Client::send_msg(const std::string &msg) const {send(_fd, &msg, msg.size(),0);}
std::string Client::make_prefix() const {return (nickname + "!" + username + "@localhost");}
