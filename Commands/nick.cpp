#include "../Server/Server.hpp"

void    Server::Nick(Client &current, std::string receive) 
{
    std::stringstream ss(receive);
    std::string command;
    std::string arg;

    ss >> command;
    ss >> arg;
    if (!current.is_passed())
        return (current.send_msg(current.msg_host(ERR_NOTREGISTERED) + " NICK :You have not registered\r\n"));

    if (arg.empty())
        return (current.send_msg(current.msg_host(ERR_NEEDMOREPARAMS) + " NICK :Not enough parameters\r\n"));
    
    std::map<int, Client>::iterator it = Clients.begin();
    while (it != Clients.end())
	{
		if (it->second.get_Nickname() == arg)
			return (current.send_msg(current.msg_host(ERR_NICKNAMEINUSE) + " " + arg + " :Nickname is already in use\r\n"));
        it++;
	}
    current.set_Has_nick(true);
    if (current.is_Registered())
    {
        current.send_msg(":" + current.make_prefix() + " NICK : " + arg + "\r\n");
        current.set_Nickname(arg);
    }

    else if (current.hasUser())
    {
        current.set_Nickname(arg);
        current.set_Registered(true);
        current.send_msg(current.msg_host(RPL_WELCOME) + " :Welcome to the Internet Relay Network " + current.make_prefix() + "\r\n");
    }
    else {}
    
}
