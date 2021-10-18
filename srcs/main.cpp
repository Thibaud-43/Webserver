#include "Cluster.hpp"
#include "Tree.hpp"

void    functionIte(std::vector<std::string>::iterator &it)
{
    for (unsigned int i = 0; i < 2; i++)
        it++;
}
int main(int argc, char const *argv[])
{
    Cluster test;

    test.run();

    // std::cout << "--------PARSING TESTS--------" << std::endl << std::endl;
    // Tree   tree("test.conf");

    // std::cout << tree;
    // tree.parseCluster();

    // return 0;
}
