#include "../Server/Server.hpp"

void    Server::Man(Client &current, std::string receive)
{
    std::stringstream ss(receive);
    std::string man;
    std::string command;
    ss >> man >> command;

    std::string prefix = ":localhost NOTICE " + current.get_Nickname();
	if (command.empty())
	{
		current.send_msg(prefix + " :Available commands: PASS, NICK, USER, PRIVMSG, JOIN, INVITE, KICK, TOPIC, MODE, PART\r\n");
		current.send_msg(prefix + " :Use MAN <command> to see how to use a specific command.\r\n");
		return;
	}
    size_t i = 0;
    while(i < command.length())
    {
        command[i] = toupper(command[i]);
        i++;
    }
	if (command == "PASS")
        current.send_msg(prefix + " :PASS <password> - Sets the connection password. Example: PASS mySecret42\r\n");
    else if (command == "NICK")
        current.send_msg(prefix + " :NICK <nickname> - Changes your nickname. Example: NICK coolUser\r\n");
    else if (command == "USER")
        current.send_msg(prefix + " :USER <username> <hostname> <servername> <realname> - Sets your user details. Example: USER ali 0 * :Ali Ben\r\n");
    else if (command == "PRIVMSG")
        current.send_msg(prefix + " :PRIVMSG <receiver>{,<receiver>} <text to be sent> - Sends a message. Example: PRIVMSG Alex :yo what's up\r\n");
    else if (command == "JOIN")
        current.send_msg(prefix + " :JOIN <channel>{,<channel>} [<key>{,<key>}] - Joins a channel. Example: JOIN #devroom\r\n");
    else if (command == "INVITE")
        current.send_msg(prefix + " :INVITE <nickname> <channel> - Invites a user to a channel. Example: INVITE Sara #devroom\r\n");
    else if (command == "KICK")
        current.send_msg(prefix + " :KICK <channel> <user> [<reason>] - Kicks a user from a channel. Example: KICK #devroom mike :breaking rules\r\n");
    else if (command == "TOPIC")
        current.send_msg(prefix + " :TOPIC <channel> [<topic>] - Changes or views the topic of a channel. Example: TOPIC #devroom :weekly meeting notes\r\n");
    else if (command == "MODE")
    {
        current.send_msg(prefix + " :MODE <channel> [<flags>] [<args>] - Changes or displays channel modes. Example: MODE #devroom -it\r\n");
        current.send_msg(prefix + " :Flags: +i (invite-only), +t (topic restricted), +k (password), +o (operator), +l (user limit)\r\n");
    }
    else if (command == "PART")
        current.send_msg(prefix + " :PART <channel>{,<channel>} [<reason>] - Leaves a channel. Example: PART #devroom :see you later\r\n");
    else
        current.send_msg(prefix + " :No manual entry for " + command + "\r\n"); 
}
