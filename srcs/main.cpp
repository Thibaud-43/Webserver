#include "Cluster.hpp"
#include "Tree.hpp"

int main(int argc, char const *argv[])
{
    if (argc == 1)
    {
        Cluster cluster;
        if (cluster.getServers().size() == 0)
            return 0;
    // test.run();
    }
    else if (argc == 2)
    {
        Cluster cluster(argv[1]);
        if (cluster.getServers().size() == 0)
            return 0;
    // test.run();
    }
    else
    {
        std::cerr << "ERROR - Wrong arguments number." << std::endl;
    }
    return 0;
}
