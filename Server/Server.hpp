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
#define MAX_EVENT 32
#define BUFFER_SIZE 1024

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

    void    handlePass(Client &, std::string receive);
	void    handleNick(Client &, std::string receive);
	void    handleUser(Client &, std::string receive);
	void    handleJoin(Client &, std::string receive);
	void    handlePart(Client &, std::string receive);
	void    handlePrivmsg(Client &, std::string receive);
	void    handleMan(Client &, std::string receive);
	void    handleTopic(Client &, std::string receive);
	void    handleInvite(Client &, std::string receive);
	void    handleKick(Client &, std::string receive);
	void    handleMode(Client &, std::string receive);

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
