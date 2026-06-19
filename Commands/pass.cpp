#include "../Server/Server.hpp"

void    Server::Pass(Client &current, std::string receive)
{
    std::stringstream ss(receive);
    std::string command;
    std::string arg;
    ss >> command;
    ss >> arg; 
    
    if (arg.empty())
        current.send_msg(":localhost " + ERR_NEEDMOREPARAMS + current.get_Nickname() + " PASS :Not enough parameters\r\n");

    else if (current.is_Registered())
        current.send_msg(":localhost " + ERR_ALREADYREGISTRED + current.get_Nickname() + " PASS :Unauthorized command, already registered\r\n");

    else if (current.is_passed())
        current.send_msg(":localhost " + ERR_ALREADYREGISTRED  + current.get_Nickname() + " PASS :password already set\r\n");

    else if (arg != _password)
        current.send_msg(":localhost " + ERR_PASSWDMISMATCH + current.get_Nickname() + " PASS :Password is incorrect\r\n");

    else
	{
		current.set_Pass(true);
		std::cout << "Client (fd = " << current.getFd() << "): Correct password. Welcome " << std::endl;
	}
}
