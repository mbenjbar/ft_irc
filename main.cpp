#include "Server/Server.hpp"


int run = 1;
void handle_signal(int) {run = 0;}


int main(int ac, char **av)
{
    int port;
    std::string password;
    if (ac != 3)
    {
        std::cout << "try again: /ircserv <port> <password>" << std::endl;
        return (1);
    }
    signal(SIGINT, handle_signal);
	signal(SIGQUIT, handle_signal);
    port =  atoi(av[1]);
    password = av[2];
    if (port < 1) return (std::cout << "invalid password\n", 1);
    try
	{
		Server server(port, password);
		server.run(run);
	}
	catch (const std::exception &exc)
	{
        std::cout << "Failure: " << exc.what() << std::endl;
		return 1;
	}
    return 0;
}