#include "../Server/Server.hpp"

void Server::handle_part(Client &current, std::string name, std::string reason)
{
    if (Channels.find(name) == Channels.end())
		{return (current.send_msg(current.msg_host(ERR_NOSUCHCHANNEL) + " " + name + " :No such channel\r\n"));}

    Channel &channel = Channels.find(name)->second;
    if (channel.is_user(current.getFd()) == false) 
        {return (current.send_msg(current.msg_host(ERR_NOTONCHANNEL) + " " + name + " :You're not on that channel\r\n"));}

	channel.broadcast(":" + current.make_prefix() + " PART " + name + (reason.empty() ? "" : " :" + reason) +  "\r\n", -1);

    channel.remove_user(current.getFd());

    if (channel.get_nbr_of_users() == 0) Channels.erase(name);
}
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
        if (reason[0] == ':') reason.erase(0, 1);
        std::string more;
        getline(ss, more);
        if (!more.empty()) reason = reason + more;
    }
    ss.str(arg);
    std::string name;
    while (getline(ss, name, ','))
    {
        size_t start = name.find_first_not_of(" \t");
		size_t end = name.find_last_not_of(" \t");
        if (start == std::string::npos)
			continue;
		name = name.substr(start, end - start + 1);
        handle_part(current, name, reason);
    }
}