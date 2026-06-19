#include "../Server/Server.hpp"

void    Server::Invite(Client &current, std::string receive)
{
    std::stringstream ss(receive);
    std::string command, target, cha_name;
    

    if (!current.is_Registered())
        return (current.send_msg(current.msg_host(ERR_NOTREGISTERED) + " INVITE :You have not registered\r\n"));

    ss >> command >> target >> cha_name;
     if (target.empty() || cha_name.empty())
        return (current.send_msg(current.msg_host(ERR_NEEDMOREPARAMS) + " INVITE :Not enough parameters\r\n"));

    if (Channels.find(cha_name) == Channels.end())
		{return (current.send_msg(current.msg_host(ERR_NOSUCHCHANNEL) + " " + cha_name + " :No such channel\r\n"));}
    Channel &chan = Channels.find(cha_name)->second;

    std::map<int, Client>::iterator it = Clients.begin();
    Client *to_invite;
    while (it != Clients.end())
    {
        if (it->second.get_Nickname() == target)
        {
            to_invite = &it->second;
            break;
        }
        it++;
    }

    if (it == Clients.end())
        {return (current.send_msg(current.msg_host(ERR_NOSUCHNICK) + " " + target + " :No such nick/channel\r\n"));}
    
    if (chan.is_user(current.getFd()) == false)
        {return (current.send_msg(current.msg_host(ERR_NOTONCHANNEL) + " " + cha_name + " :You're not on that channel\r\n"));}

    if (chan.get_invit_only() && chan.is_op(current.getFd()) == false)
        {return (current.send_msg(current.msg_host(ERR_CHANOPRIVSNEEDED) + " " + cha_name + " :You're not on that channel\r\n"));}
    
    if (chan.is_user(to_invite->getFd()))
        {return (current.send_msg(current.msg_host(ERR_USERONCHANNEL) + " " + to_invite->get_Nickname() + " " + cha_name + " :is already on channel\r\n"));}

    chan.add_invit(to_invite->getFd());

    current.send_msg(current.msg_host(RPL_INVITING) + " " + to_invite->get_Nickname() + " " + cha_name + "\r\n");

    to_invite->send_msg(":" + current.make_prefix() + " INVITE " + to_invite->get_Nickname() + " :" + cha_name + "\r\n");
}
