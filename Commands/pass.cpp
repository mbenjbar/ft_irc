#include "../Server/Server.hpp"

void    Server::Pass(Client &current, std::string receive)
{
    std::stringstream ss(receive);
    std::string command;
    std::string arg;
    ss >> command;
    ss >> arg; 
    
    if (arg.empty())
        current.send_msg(current.msg_host(ERR_NEEDMOREPARAMS) + " PASS :Not enough parameters\r\n");

    else if (current.is_Registered())
        current.send_msg(current.msg_host(ERR_ALREADYREGISTRED) + " PASS :Unauthorized command, already registered\r\n");

    else if (current.is_passed())
        current.send_msg(current.msg_host(ERR_ALREADYREGISTRED) + " PASS :password already set\r\n");

    else if (arg != _password)
        current.send_msg(current.msg_host(ERR_PASSWDMISMATCH) + " PASS :Password is incorrect\r\n");

    else
	{
		current.set_Pass(true);
		std::cout << "Client (fd = " << current.getFd() << "): Correct password. Welcome " << std::endl;
	}
}
