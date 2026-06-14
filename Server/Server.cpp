#include "Server.hpp"

Server::~Server()
{
    if (fd_epoll >= 0)
        close (fd_epoll);
    if (fd_server >= 0)
        close (fd_server);
    Clients.clear();
}

Server::Server(int port, std::string password)
{
    _port = port;
    _password = password;
    fd_server = -1;
    fd_epoll = -1;

    fd_server = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_server< 0)
        throw Serv_Exception("socket Failed");
    int option = 1;
    if (setsockopt(fd_server, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0) //socket address can be reused, no waiting opt = 1 -> enable SO_REUSEADDR 
    {
        close(fd_server);
        throw Serv_Exception("setsockopt Failed");
    }

    if (fcntl(fd_server, F_SETFL, O_NONBLOCK) < 0) // Non blocking fd: don't wait or freeze in read/write/recv..
    {
        close(fd_server);
        throw Serv_Exception("fcntl Failed");
    }

    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY; // any address
    addr.sin_port = htons(_port);

    if (bind(fd_server, (sockaddr *)&addr, sizeof(addr)) < 0)
    {
        close(fd_server);
        throw Serv_Exception("bind Failed");
    }

    if (listen(fd_server, SOMAXCONN) < 0) //SOMAXCONN is just a constant that tells the OS: Use max allowed backlog queue for incoming connections
    {
        close(fd_server);
        throw Serv_Exception("listen Failed"); 
    }
}

void Server::run(int &run)
{
    fd_epoll = epoll_create(1);
    if (fd_epoll < 0)
        throw Serv_Exception("epoll_create Failed");
    epoll_event event_ctrl;
    event_ctrl.events = EPOLLIN;
    event_ctrl.data.fd = fd_server;
    if (epoll_ctl(fd_epoll, EPOLL_CTL_ADD, fd_server, &event_ctrl) < 0)
        throw Serv_Exception("epoll_ctl Failed");
    std::cout << "Server listening on port " << _port << "..." << std::endl;

    epoll_event events[MAX_EVENT];
    int fd_client = -1;
    sockaddr_in client_addr;
    while (run == 1)
    {
        int event_hap = epoll_wait(fd_epoll, events, MAX_EVENT, -1); // -1 means wait infinetly (otherwise it is the max t to wait in ms)
        if (event_hap < 0)
        {
            if (errno == EINTR)
                continue; //signal interrupted the server like ctrl+c but gotta keep server workinng
        }
    }

}


// Exception FCT

Server::Serv_Exception::Serv_Exception(std::string error_msg): msg(error_msg) {}
const char *Server::Serv_Exception::what() const throw() {return msg.c_str();}
Server::Serv_Exception::~Serv_Exception() throw() {}