#include "../Server/Server.hpp"

void handle_i(Client &current, Channel &chan, bool add)
{
    chan.set_invit_only(add);
    chan.broadcast(":" + current.make_prefix() + " MODE " + chan.get_name() + (add ? " +i" : " -i") + "\r\n", -1);
}

void handle_t(Client &current, Channel &chan, bool add)
{
    chan.set_topic_ch_restric(add);
    chan.broadcast(":" + current.make_prefix() + " MODE " + chan.get_name() + (add ? " +t" : " -t") + "\r\n", -1);
}

void handle_k(Client &current, Channel &chan, bool add, std::vector<std::string> &args, size_t &arg_i)
{
    if (add == true)
    {
        if (arg_i >= args.size())
            return (current.send_msg(current.msg_host(ERR_NEEDMOREPARAMS) + " MODE :Not enough parameters\r\n"));
        chan.set_password(args[arg_i]);
        chan.broadcast(":" + current.make_prefix() + " MODE " + chan.get_name() + " +k " + chan.get_password() + "\r\n", -1);
        arg_i++;
    }
    else
    {
        chan.remove_password();
        chan.broadcast(":" + current.make_prefix() + " MODE " + chan.get_name() + " -k" + "\r\n", -1);
    }
}

void handle_l(Client &current, Channel &chan, bool add, std::vector<std::string> &args, size_t &arg_i)
{
    if (add == true)
    {
        if (arg_i >= args.size())
            return (current.send_msg(current.msg_host(ERR_NEEDMOREPARAMS) + " MODE :Not enough parameters\r\n"));
        int limit = atoi(args[arg_i].c_str());
        chan.set_members_limit(limit);
        chan.broadcast(":" + current.make_prefix() + " MODE " + chan.get_name() + " +l " + args[arg_i] + "\r\n", -1);
        arg_i++;
    }
    else
    {
        chan.remove_members_limit();
        chan.broadcast(":" + current.make_prefix() + " MODE " + chan.get_name() + " -l" + "\r\n", -1);
    }
}

void handle_o(Client &current, Channel &chan, bool add, std::vector<std::string> &args, size_t &arg_i)
{
    if (arg_i >= args.size())
        return (current.send_msg(current.msg_host(ERR_NEEDMOREPARAMS) + " MODE :Not enough parameters\r\n"));
    std::map<int, Client *>::const_iterator it;
    const std::map<int, Client *> &users = chan.get_users();
    it = users.begin();
    Client *to_op = NULL;
    std::string target = args[arg_i];
    while (it != users.end())
    {
        if (it->second->get_Nickname() == target)
        {
            to_op = it->second;
            break;
        }
        it++;
    }
    if (!to_op)
        {return (current.send_msg(current.msg_host(ERR_USERNOTINCHANNEL) + " " + target + " " + chan.get_name() + " :They aren't on that channel\r\n"));}
    if (add == true)
    {
        chan.add_op(*to_op);
        chan.broadcast(":" + current.make_prefix() + " MODE " + chan.get_name() + " +o " + target + "\r\n", -1);
    }   
    else
    {
        chan.remove_op(to_op->getFd());
        chan.broadcast(":" + current.make_prefix() + " MODE " + chan.get_name() + " -o " + target + "\r\n", -1);
    } 
    arg_i++;
}

void    Server::Mode(Client &current, std::string receive)
{
    std::stringstream ss(receive);
    std::string command;
    std::string channel_name;
    std::string mode;
    std::vector<std::string> args;

    if (!current.is_Registered())
        return (current.send_msg(current.msg_host(ERR_NOTREGISTERED) + " MODE :You have not registered\r\n"));
    ss >> command >> channel_name >> mode;
    if (channel_name.empty())
        return (current.send_msg(current.msg_host(ERR_NEEDMOREPARAMS) + " MODE :Not enough parameters\r\n"));

    if (Channels.find(channel_name) == Channels.end())
		{return (current.send_msg(current.msg_host(ERR_NOSUCHCHANNEL) + " " + channel_name + " :No such channel\r\n"));}
    
    Channel &chan = Channels.find(channel_name)->second;
    if (mode.empty()) // view mode
    {
        std::string reply;
        if (chan.get_invit_only())
            reply += "i";
        if (chan.get_topic_ch_restrict())
            reply += "t";
        if (chan.get_has_pswd())
            reply += "k";
        if (chan.get_limited())
        {
            reply += "l";
            std::stringstream tmp;
            tmp << chan.get_members_limit();
            reply += " " + tmp.str();
        }
        if (!reply.empty())
            reply = current.msg_host(RPL_CHANNELMODEIS) + " " + channel_name + " +" + reply + "\r\n";
        else
            reply = current.msg_host(RPL_CHANNELMODEIS) + " " + channel_name + "\r\n";
        current.send_msg(reply);
        return;
    }

    if (chan.is_op(current.getFd()) == false)
        {return (current.send_msg(current.msg_host(ERR_CHANOPRIVSNEEDED) + " " + channel_name + " :You're not channel operator\r\n"));}
    std::string tmp;
    while (ss >> tmp) args.push_back(tmp);

    size_t i = 0;
    size_t arg_i = 0;
    bool add = true;
    while (i < mode.size())
    {
        if (mode[i] == '+') {add = true; i++; continue;}
        if (mode[i] == '-') {add = false; i++; continue;}
        if (mode[i] == 'i') handle_i(current, chan, add);
        else if (mode[i] == 't') handle_t(current, chan, add);
        else if (mode[i] == 'k') handle_k(current, chan, add, args, arg_i);
        else if (mode[i] == 'l') handle_l(current, chan, add, args, arg_i);
        else if (mode[i] == 'o') handle_o(current, chan, add, args, arg_i);
        else current.send_msg(current.msg_host(ERR_UNKNOWNMODE) + " " + mode[i] + " :is unknown mode char to me\r\n");
        i++;
    }
}