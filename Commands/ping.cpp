#include "../Server/Server.hpp"

void    Server::Ping(Client &current, std::string receive)
{
    std::stringstream ss(receive);
    std::string command;
    std::string token;

    ss >> command >> token;
    if (token[0] == ':')
        token.erase(0, 1);
    if (!token.empty())
        current.send_msg("PONG :" + token + "\r\n");
}