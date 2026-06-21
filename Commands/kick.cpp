#include "../Server/Server.hpp"

void    Server::Kick(Client &current, std::string receive)
{
    std::stringstream ss(receive);
    std::string command;
    std::string channel_name;
    std::string target;

    if (!current.is_Registered())
        return (current.send_msg(current.msg_host(ERR_NOTREGISTERED) + " KICK :You have not registered\r\n"));
    ss >> command >> channel_name >> target;
    if (channel_name.empty() || target.empty())
        return (current.send_msg(current.msg_host(ERR_NEEDMOREPARAMS) + " KICK :Not enough parameters\r\n"));

    if (Channels.find(channel_name) == Channels.end())
		{return (current.send_msg(current.msg_host(ERR_NOSUCHCHANNEL) + " " + channel_name + " :No such channel\r\n"));}
    
    Channel &chan = Channels.find(channel_name)->second;
    std::string reason;
    std::getline(ss >> std::ws, reason);
    if (!reason.empty() && reason[0] == ':') reason.erase(0, 1);
    if (reason.empty()) reason = "Kicked";

    std::map<int, Client>::iterator it = Clients.begin();
    Client *to_kick = NULL;
    while (it != Clients.end())
    {
        if (it->second.get_Nickname() == target)
        {
            to_kick = &it->second;
            break;
        }
        it++;
    }
    if (!to_kick)
        {return (current.send_msg(current.msg_host(ERR_NOSUCHNICK) + " " + target + " :No such nick/channel\r\n"));}
    
    if (chan.is_user(current.getFd()) == false)
        {return (current.send_msg(current.msg_host(ERR_NOTONCHANNEL) + " " + channel_name + " :You're not on that channel\r\n"));}

    if (chan.is_op(current.getFd()) == false)
        {return (current.send_msg(current.msg_host(ERR_CHANOPRIVSNEEDED) + " " + channel_name + " :You're not channel operator\r\n"));}
    
    if (chan.is_user(to_kick->getFd()) == false)
        {return (current.send_msg(current.msg_host(ERR_USERNOTINCHANNEL) + " " + to_kick->get_Nickname() + " " + channel_name + " :They aren't on that channel\r\n"));}
    
    chan.broadcast(":" + current.make_prefix() + " KICK " + channel_name + " " + to_kick->get_Nickname() + " :" + reason + "\r\n", -1);
    chan.remove_user(to_kick->getFd());
}