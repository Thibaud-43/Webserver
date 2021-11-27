#include "Cluster.hpp"
#include "Tree.hpp"
#include "Response.hpp"

void    leave(int sig)
{
    (void)sig;
    ASocket::clear();
    std::cout << "[" << HTTPDate() << "]" << ": server shutdown\n";
    close(FileDescriptor::getEpollFd());
    throw std::exception();
}

int main(int argc, char const *argv[])
{
    signal(SIGINT, leave);
    signal(SIGKILL, leave);
    signal(SIGTERM, leave);
    signal(SIGSTOP, leave);
    if (argc == 1)
    {
        Cluster cluster;
        if (cluster.getServers().size() == 0)
            return 1;
        try
        {
            cluster.run();
        }
        catch(const std::exception& e)
        {
		    ASocket::clear();
            return 0;
        }
    }
    else if (argc == 2)
    {
        Cluster cluster(argv[1]);
        if (cluster.getServers().size() == 0)
            return 1;
        try
        {
            cluster.run();
        }
        catch(const std::exception& e)
        {
		    ASocket::clear();
            return 0;
        }
    }
    else
    {
        std::cerr << "ERROR - Wrong arguments number." << std::endl;
        return 2;
    }
    return 0;
}
