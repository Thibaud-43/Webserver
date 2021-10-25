#include "Cluster.hpp"
#include "Tree.hpp"

int main(int argc, char const *argv[])
{
    if (argc == 1)
    {
        std::cout << "DEFAULT FILE" << std::endl;
        Cluster cluster;
        if (cluster.getServers().size() == 0)
            return 0;
        std::cout << "RUN" << std::endl;
    }
    else if (argc == 2)
    {
        std::cout << "ARG FILE" << std::endl;
        Cluster cluster(argv[1]);
        if (cluster.getServers().size() == 0)
            return 0;
        std::cout << "RUN" << std::endl;
    }
    // test.run();

    return 0;
}
