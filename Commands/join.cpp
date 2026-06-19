#include "../Server/Server.hpp"

static std::vector<std::string> split(const std::string &str, char c)
{
    std::vector<std::string> splited;
    std::istringstream ss(str);
    std::string tmp;
    while (std::getline(ss, tmp, c))
        splited.push_back(tmp);
    return (splited);
}
void print_users(Client &current, Channel &channel)
{
    std::string list;
    std::map<int, Client*>::const_iterator it;
    const std::map<int, Client*> &members = channel.get_users();
    it = members.begin();
    while (it != members.end())
    {
        if (!list.empty())
            list = list + " ";
        if (channel.is_op(it->first))
            list = list + "@";
        list = list + it->second->get_Nickname();
        it++;
    }

    current.send_msg(current.msg_host(RPL_NAMREPLY) + " = " + channel.get_name() + " :" + list + "\r\n");
    current.send_msg(current.msg_host(RPL_ENDOFNAMES) + " " + channel.get_name() + " :End of /NAMES list\r\n");
}
void Server::handle_join(Client &current, std::string channel_name, std::string pass)
{
    if (channel_name.empty() || (channel_name[0] != '#' && channel_name[0] != '&' && channel_name[0] != '+' && channel_name[0] != '!')
        || channel_name.find(' ') != std::string::npos || channel_name.find(',') != std::string::npos)
    {
        return (current.send_msg(current.msg_host(ERR_BADCHANMASK) + " " + channel_name + " :Invalid channel name\r\n"));
    }
    
    if (Channels.find(channel_name) == Channels.end()) Channels.insert(std::make_pair(channel_name, Channel(channel_name)));
    
    Channel &tmp = Channels.find(channel_name)->second;

    if (tmp.is_user(current.getFd())) {std::cout << "Client " << current.get_Nickname() << " already on channel " << channel_name << std::endl;
        return;}

    if (tmp.get_has_pswd() && tmp.get_password() != pass)
        return (current.send_msg(current.msg_host(ERR_BADCHANNELKEY) + " " + channel_name + " :Cannot join channel (+k)\r\n"));
    
    if (tmp.get_limited() && (int)tmp.get_nbr_of_users() >= tmp.get_members_limit())
        return (current.send_msg(current.msg_host(ERR_CHANNELISFULL) + " " + channel_name + " :Cannot join channel (+l)\r\n"));

    if (tmp.get_invit_only() && !tmp.is_invit(current.getFd()))
        return (current.send_msg(current.msg_host(ERR_INVITEONLYCHAN) + " " + channel_name + " :Cannot join channel (+i)\r\n"));

    tmp.add_user(current);
    if (tmp.get_nbr_of_users() == 1) tmp.add_op(current);
    if (tmp.is_invit(current.getFd())) tmp.remove_invit(current.getFd());

    tmp.broadcast(":" + current.make_prefix() + " JOIN :" + channel_name + "\r\n", -1);

    if (tmp.get_topic().empty())
        current.send_msg(current.msg_host(RPL_NOTOPIC) + " " + channel_name + " :No topic is set\r\n");
    else
        current.send_msg(current.msg_host(RPL_TOPIC) + " " + channel_name + " :" + tmp.get_topic() + "\r\n");
    
    print_users(current, Channels.find(channel_name)->second);
}
void    Server::Join(Client &current, std::string receive)
{
    std::stringstream ss(receive);
    std::string command;
    std::string chan_tmp;
    std::string pass_tmp;
    std::vector<std::string> chans;
    std::vector<std::string> passes;
    ss >> command >> chan_tmp >> pass_tmp;

    if (!current.is_Registered())
        return (current.send_msg(current.msg_host(ERR_NOTREGISTERED) + " JOIN :You have not registered\r\n"));
    if (chan_tmp.empty())
        return (current.send_msg(current.msg_host(ERR_NEEDMOREPARAMS) + " JOIN :Not enough parameters\r\n"));
    chans = split(chan_tmp, ',');
    passes = split(pass_tmp, ',');
    size_t i = 0;
    while ( i < chans.size())
    {
        chan_tmp = chans[i];

        pass_tmp = "";
        if (i < passes.size())
            pass_tmp = passes[i];

        handle_join(current, chan_tmp, pass_tmp);
        i++;
    }
}