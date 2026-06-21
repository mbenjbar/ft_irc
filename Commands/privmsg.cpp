#include "../Server/Server.hpp"

void    Server::Privmsg(Client &current, std::string receive)
{
    std::stringstream ss(receive);
    std::string command;
    std::string target;
    std::string msg;

    if (!current.is_Registered())
        return (current.send_msg(current.msg_host(ERR_NOTREGISTERED) + " PRIVMSG :You have not registered\r\n"));
    
    ss >> command >> target;
    if (target.empty())
        return (current.send_msg(current.msg_host(ERR_NEEDMOREPARAMS) + " PRIVMSG :Not enough parameters\r\n"));

    std::getline(ss >> std::ws, msg);
    if (!msg.empty() && msg[0] == ':') msg.erase(0, 1);
    if (msg.empty())
        return (current.send_msg(current.msg_host(ERR_NOTEXTTOSEND) + " :No text to send\r\n"));
    std::stringstream ss2(target);
    std::string tmp_target;
    while (getline(ss2, tmp_target, ','))
    {
        std::string new_msg = current.make_prefix() + " PRIVMSG " + tmp_target + " :" + msg + "\r\n";
        if (tmp_target[0] == '#' || tmp_target[0] == '&') // send to channel
        {
            if (Channels.find(tmp_target) == Channels.end())
		        {current.send_msg(current.msg_host(ERR_NOSUCHCHANNEL) + " " + tmp_target + " :No such channel\r\n"); continue;}
    
            Channel &chan = Channels.find(tmp_target)->second;
            if (chan.is_user(current.getFd()) == false)
                {current.send_msg(current.msg_host(ERR_CANNOTSENDTOCHAN) + " " + tmp_target + " :Cannot send to channel\r\n"); continue;}

            chan.broadcast(new_msg, current.getFd());
        }
        else // send to a user
        {
            std::map<int, Client>::iterator it = Clients.begin();
            Client *to_text = NULL;
            while (it != Clients.end())
            {
                if (it->second.get_Nickname() == tmp_target)
                {
                    to_text = &it->second;
                    to_text->send_msg(new_msg);
                    break;
                }
                it++;
            }
            if (!to_text)
                {current.send_msg(current.msg_host(ERR_NOSUCHNICK) + " " + tmp_target + " :No such nick/channel\r\n");}
        }
    }
}