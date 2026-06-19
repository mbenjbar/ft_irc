#include "../Server/Server.hpp"

void    Server::Part(Client &current, std::string receive)
{
    std::stringstream ss(receive);
    std::string command;
    std::string arg;

    ss >> command;
    ss >> arg;
    if (!current.is_Registered())
        return (current.send_msg(current.msg_host(ERR_NOTREGISTERED) + " PART :You have not registered\r\n"));

    if (arg.empty())
        return (current.send_msg(current.msg_host(ERR_NEEDMOREPARAMS) + " PART :Not enough parameters\r\n"));

    std::string reason;
    ss >> reason;
    if (!reason.empty())
    {
        if (reason[0] == ':') reason.erase(0);
        std::string more;
        getline(ss, more);
        if (!more.empty()) reason = reason + more;
    }
    ss.str(arg);

}