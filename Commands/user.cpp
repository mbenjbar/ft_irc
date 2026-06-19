#include "../Server/Server.hpp"

void    Server::User(Client &current, std::string receive)
{
    std::stringstream ss(receive);
    std::string command;
    std::string username;
    std::string realname;
    std::string host;
    std::string server;

    ss >> command;
    ss >> username;
    ss >> host;
    ss >> server;


    if (!current.is_passed())
        return (current.send_msg(":localhost " + ERR_NOTREGISTERED + current.get_Nickname() + " USER :You have not registered\r\n"));
    
    if (current.is_Registered() || current.hasUser())
        return (current.send_msg(":localhost " + ERR_ALREADYREGISTRED + current.get_Nickname() + " USER :Unauthorized command, already registered\r\n"));

    if (username.empty())
        return (current.send_msg(":localhost " + ERR_NEEDMOREPARAMS + current.get_Nickname() + " USER :Not enough parameters\r\n"));

    getline(ss, realname);
    if (!realname.empty() && realname[0] == ' ')
    realname.erase(0, 1);
    if (!realname.empty() && realname[0] == ':')
    realname.erase(0, 1);
    

    current.set_Username(username);
	current.set_Realname(realname);
	current.set_Has_user(true);

    if (current.Nick_named())
	{
		current.set_Registered(true);
        current.send_msg(":localhost " + RPL_WELCOME + current.get_Nickname() + " :Welcome to the Internet Relay Network " + current.get_Nickname() + "!" + current.get_Username() + "@localhost\r\n");
	}
	else {}
}