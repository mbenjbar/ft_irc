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
#define MAX_EVENT 10

class Server
{
private:
    int fd_server;
    int fd_epoll;
    bool running_state;
    int _port;
    std::string _password;
    std::map<int , Client> Clients;
    std::map<std::string, Channel> Channels;

public:
    Server(int port, std::string password);
    ~Server();

    void run(int &run);

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
