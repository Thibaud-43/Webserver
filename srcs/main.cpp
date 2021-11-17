#include "Cluster.hpp"
#include "Tree.hpp"
#include "Response.hpp"

void    leave(int sig)
{
    (void)sig;
    ASocket::clear();
    std::cout << "[" << HTTPDate() << "]" << ": server shutdown\n";
    close(FileDescriptor::getEpollFd());
    exit(0);
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
            return 0;
        cluster.run();
    }
    else if (argc == 2)
    {
        Cluster cluster(argv[1]);
        if (cluster.getServers().size() == 0)
            return 0;
        cluster.run();
    }
    else
    {
        std::cerr << "ERROR - Wrong arguments number." << std::endl;
    }
    return 0;
}
