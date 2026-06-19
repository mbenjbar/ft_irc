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
        return (current.send_msg(current.msg_host(ERR_NOTREGISTERED) + " USER :You have not registered\r\n"));
    
    if (current.is_Registered() || current.hasUser())
        return (current.send_msg(current.msg_host(ERR_ALREADYREGISTRED) + " USER :Unauthorized command, already registered\r\n"));

    if (username.empty())
        return (current.send_msg(current.msg_host(ERR_NEEDMOREPARAMS) + " USER :Not enough parameters\r\n"));

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
        current.send_msg(current.msg_host(RPL_WELCOME) + " :Welcome to the Internet Relay Network " + current.make_prefix() + "\r\n");
	}
	else {}
}