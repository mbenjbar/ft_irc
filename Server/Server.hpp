#pragma once

#include "../Client/Client.hpp"
#include "../Channel/Channel.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <unistd.h> // read() write() close()
#include <errno.h>
#include <sys/socket.h> // recv() send() socket() bind() listen()
#include <netinet/in.h> // port IP and internet address: sockaddr_in, INADDR_ANY, htons, ntohs
#include <arpa/inet.h> // IP conversion: inet_ntoa() 
#include <fcntl.h> // control fd behavior
#include <sys/epoll.h> // events control: multiplexing
#include <vector> // Container
#include <map>  // Container
#include <cctype> // isdigit() toupper()
#include "string.h"
#include <cstdio>
#include <cstdlib>
#include <csignal>
#define MAX_EVENT 32
#define BUFFER_SIZE 1024
#define ERR_NEEDMOREPARAMS std::string("461 ")
#define ERR_ALREADYREGISTRED std::string("462 ")
#define ERR_PASSWDMISMATCH std::string("464 ")
#define ERR_NICKNAMEINUSE std::string("433 ")
#define RPL_WELCOME std::string("001 ")
#define ERR_NOTREGISTERED std::string("451 ")
#define ERR_NOSUCHCHANNEL std::string("403 ")
#define ERR_BADCHANMASK std::string("476 ")
#define ERR_BADCHANNELKEY std::string("475 ")
#define ERR_CHANNELISFULL std::string("471 ")
#define ERR_INVITEONLYCHAN std::string("473 ")
#define RPL_NOTOPIC std::string("331 ")
#define RPL_TOPIC std::string("332 ")
#define RPL_NAMREPLY std::string("353 ")
#define RPL_ENDOFNAMES std::string("366 ")
#define ERR_NOTONCHANNEL std::string("442 ")
#define ERR_NOSUCHNICK std::string("401 ")
#define ERR_CHANOPRIVSNEEDED std::string("482 ")
#define ERR_USERONCHANNEL std::string("443 ")
#define RPL_INVITING std::string("341 ")



class Channel; //circular include loop between Channel.hpp and Server.hpp

class Server
{
private:
    int fd_server;
    int fd_epoll;
    int _port;
    std::string _password;
    std::map<int , Client> Clients;
    std::map<std::string, Channel> Channels;
    int     new_client();
    void    handle_buff(Client &);
    void    handle_command(std::string, Client &);

    void    Pass(Client &, std::string receive);
	void    Nick(Client &, std::string receive);
	void    User(Client &, std::string receive);
	void    Join(Client &, std::string receive);
    void    handle_join(Client &, std::string channel_name, std::string pass);
	void    Part(Client &, std::string receive);
    void    handle_part(Client &, std::string channel_name, std::string reason);
	void    Privmsg(Client &, std::string receive);
	void    Man(Client &, std::string receive);
	void    Topic(Client &, std::string receive);
	void    Invite(Client &, std::string receive);
	void    Kick(Client &, std::string receive);
	void    Mode(Client &, std::string receive);
    void    Ping(Client &, std::string receive);

public:
    Server(int port, std::string password);
    ~Server();
    void    run(int &run);

    class Serv_Exception : public std::exception
    {
        private:
            std::string msg;
        public:
            Serv_Exception(std::string error_msg);
            const char *what() const throw();
            ~Serv_Exception() throw();
    };    
};
