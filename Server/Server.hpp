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
	void    Part(Client &, std::string receive);
	void    Privmsg(Client &, std::string receive);
	void    Man(Client &, std::string receive);
	void    Topic(Client &, std::string receive);
	void    Invite(Client &, std::string receive);
	void    Kick(Client &, std::string receive);
	void    Mode(Client &, std::string receive);

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
