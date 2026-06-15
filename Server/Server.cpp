#include "Server.hpp"

Server::~Server()
{
    if (fd_epoll)
        close (fd_epoll);
    if (fd_server)
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
    while (run == 1)
    {
        int event_hap = epoll_wait(fd_epoll, events, MAX_EVENT, -1); // -1 means wait infinetly (otherwise it is the max t to wait in ms)
        if (event_hap < 0)
        {
            if (errno == EINTR)
                continue; //signal interrupted the server like ctrl+c but gotta keep server workinng
        }
        int i = 0;
        while (i++ < event_hap)
        {
            if (events[i].data.fd == fd_server)
                if (new_client() < 0)
                    continue;
            else if (events[i].events & EPOLLIN)
            {
                int fd_recv = events[i].data.fd;
                char buffer[1024];
                int bytes = recv(fd_recv, &buffer, sizeof(buffer) - 1, 0);
                if (bytes <= 0)
                {
                    std::cout << "client with fd: " << fd_recv <<" disconnected" << std::endl;
                    epoll_ctl(fd_epoll, EPOLL_CTL_DEL, fd_recv, NULL);
                    close(fd_recv);
                    if (Clients.find(fd_recv) != Clients.end()) Clients.erase(fd_recv);
                    continue;
                }
                buffer[bytes] = '\0';
                std::map<int, Client>::iterator iter = Clients.find(fd_recv);
                if (iter != Clients.end()) {iter->second.append_buff(std::string(buffer, bytes)); handle_buff(iter->second);}
            }
        }
    }

}

void Server::handle_buff(Client &current)
{
    std::string buf = current.get_Buffer();
    int i = buf.find('\n');
    while (i != std::string::npos)
    {
        std::string command = buf.substr(0, i); // exclude \n
        if (!command.empty()) 
        {
            if (command[command.length() - 1] == '\r') command.pop_back();
            handle_command(command, current);
        }
    }
}

void Server::handle_command(std::string receive, Client &Client)
{
    std::stringstream s(receive);
    std::string command;
    if (!(s >> command)) return;
    for (int i = 0; i < command.size(); i++) command[i] = toupper(command[i]);
    std::string msg = ":localhost 421 " + Client.get_Nickname() + " " + command + " :Unknown command\r\n";

    if (command == "PASS") handlePass(Client, receive);
	else if (command == "NICK") handleNick(Client, receive);
	else if (command == "USER") handleUser(Client, receive);
	else if (command == "JOIN") handleJoin(Client, receive);
	else if (command == "PART") handlePart(Client, receive);
	else if (command == "PRIVMSG") handlePrivmsg(Client, receive);
	else if (command == "MAN") handleMan(Client, receive);
	else if (command == "TOPIC") handleTopic(Client, receive);
	else if (command == "INVITE") handleInvite(Client, receive);
	else if (command == "KICK") handleKick(Client, receive);
	else if (command == "MODE") handleMode(Client, receive);
    else Client.send_msg(msg);
}

int Server::new_client()
{
    int fd_client = -1;
    sockaddr_in client_addr;
    socklen_t client_adr_len = sizeof(client_addr);
    epoll_event event_ctrl;
    fd_client = accept(fd_server, (sockaddr *)&client_addr, &client_adr_len);
    if (fd_client < 0)
    {
        std::cerr << "accept failed" << std::endl;
        return -1;
    }
    if (fcntl(fd_client, F_SETFL, O_NONBLOCK) < 0)
    {
        close(fd_client);
        std::cerr << "fcntl failed" << std::endl;
        return -1;
    }
    event_ctrl.data.fd = fd_client;
    event_ctrl.events = EPOLLIN;
    if (epoll_ctl(fd_epoll, EPOLL_CTL_ADD, fd_client, &event_ctrl) < 0)
    {
        close(fd_client);
        std::cerr << "epoll_ctl failed" << std::endl;
        return -1;
    }
    std::cout << "Client connected from " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << std::endl;
    Clients.insert(std::make_pair(fd_client, Client(fd_client)));

}


// Exception FCT

Server::Serv_Exception::Serv_Exception(std::string error_msg): msg(error_msg) {}
const char *Server::Serv_Exception::what() const throw() {return msg.c_str();}
Server::Serv_Exception::~Serv_Exception() throw() {}