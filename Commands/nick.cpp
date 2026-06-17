#include "../Server/Server.hpp"

void    Server::Nick(Client &current, std::string receive) 
{
    std::stringstream ss(receive);
    std::string command;
    std::string arg;

    ss >> command;
    ss >> arg;
    if (!current.is_passed())
        return (current.send_msg(":localhost " + ERR_NOTREGISTERED + current.get_Nickname() + " NICK :You have not registered\r\n"));

    else if (arg.empty())
        return (current.send_msg(":localhost " + ERR_NEEDMOREPARAMS + current.get_Nickname() + " NICK :Not enough parameters\r\n"));
    
    std::map<int, Client>::iterator it = Clients.begin();
    while (it != Clients.end())
	{
		if (it->second.get_Nickname() == arg)
			return (current.send_msg(":localhost " + ERR_NICKNAMEINUSE + current.get_Nickname() + " " + arg + " :Nickname is already in use\r\n"));
        it++;
	}
    current.set_Has_nick(true);
    if (current.is_Registered()) current.send_msg(":" + current.get_Nickname() + "!" + current.get_Username() + "@localhost NICK : " + arg + "\r\n");
    else if (current.hasUser())
    {
        current.set_Registered(true);
        current.send_msg(":localhost " + RPL_WELCOME + arg + " :Welcome to the Internet Relay Network " + arg + "!" + current.get_Username() + "@localhost\r\n");
    }
    else {}
    current.set_Nickname(arg);
}
