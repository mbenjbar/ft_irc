#include "../Server/Server.hpp"

std::string &get_realname(std::stringstream &ss)
{

}

void    Server::User(Client &current, std::string receive)
{
    std::stringstream ss(receive);
    std::string command;
    std::string username;
    std::string realname;

    ss >> command;
    ss >> username;


    if (!current.is_passed())
        return (current.send_msg(":localhost " + ERR_NOTREGISTERED + current.get_Nickname() + " USER :You have not registered\r\n"));
    
    if (current.is_Registered() || current.hasUser())
        return (current.send_msg(":localhost " + ERR_ALREADYREGISTRED + current.get_Nickname() + " USER :Unauthorized command, already registered\r\n"));

    if (username.empty())
        return (current.send_msg(":localhost " + ERR_NEEDMOREPARAMS + current.get_Nickname() + " USER :Not enough parameters\r\n"));

    realname = get_realname(ss);
}