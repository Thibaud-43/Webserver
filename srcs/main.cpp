#include "Cluster.hpp"
#include "Tree.hpp"

int main(int argc, char const *argv[])
{
    if (argc == 1)
    {
        std::cout << "DEFAULT FILE" << std::endl;
        Cluster cluster;
    }
    else if (argc == 2)
    {
        std::cout << "ARG FILE" << std::endl;
        Cluster cluster(argv[1]);
    }
    // test.run();

    return 0;
}
