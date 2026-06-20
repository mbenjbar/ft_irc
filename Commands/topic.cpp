#include "../Server/Server.hpp"

void    Server::Topic(Client &current, std::string receive)
{
    std::stringstream ss(receive);
    std::string command;
    std::string channel_name;
    std::string topic;
    if (!current.is_Registered())
        return (current.send_msg(current.msg_host(ERR_NOTREGISTERED) + " TOPIC :You have not registered\r\n"));
    ss >> command >> channel_name;

    if (channel_name.empty())
        return (current.send_msg(current.msg_host(ERR_NEEDMOREPARAMS) + " TOPIC :Not enough parameters\r\n"));

    if (Channels.find(channel_name) == Channels.end())
		{return (current.send_msg(current.msg_host(ERR_NOSUCHCHANNEL) + " " + channel_name + " :No such channel\r\n"));}
    
    Channel &chan = Channels.find(channel_name)->second;
    if (chan.is_user(current.getFd()) == false)
        {return (current.send_msg(current.msg_host(ERR_NOTONCHANNEL) + " " + channel_name + " :You're not on that channel\r\n"));}

    std::getline(ss >> std::ws, topic);
    int view_mode = topic.empty();
    if (!topic.empty() && topic[0] == ':')
        topic.erase(0, 1);
    
    if (view_mode)
    {
        if (chan.get_topic().empty())
            return (current.send_msg(current.msg_host(RPL_NOTOPIC) + " " + channel_name + " :No topic is set\r\n"));
        else
            return (current.send_msg(current.msg_host(RPL_TOPIC) + " " + channel_name + " :" + chan.get_topic() + "\r\n"));
    }
    if (chan.get_topic_ch_restrict() && chan.is_op(current.getFd()) == false)
        {return (current.send_msg(current.msg_host(ERR_CHANOPRIVSNEEDED) + " " + channel_name + " :You're not channel operator\r\n"));}
    chan.set_topic(topic);
    chan.broadcast(":" + current.make_prefix() + " TOPIC " + chan.get_name() + " :" + chan.get_topic() + "\r\n", -1);
}