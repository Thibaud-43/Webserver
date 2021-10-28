#include "Cluster.hpp"
#include "Tree.hpp"

// void printBT(const std::string& prefix, const Node* node, bool isLeft)
// {
//     if( node != NULL )
//     {
//         std::cout << prefix;

//         std::cout << (isLeft ? "├──" : "└──" );

//         // print the value of the node
//         std::cout << node->getType() << std::endl;

//         // enter the next tree level - left and right branch
//         printBT( prefix + (isLeft ? "│   " : "    "), node->getRight(), true);
//         printBT( prefix + (isLeft ? "│   " : "    "), node->getLeft(), false);
//     }
// }

// void printBT(const Node* node)
// {
//     printBT("", node, false);    
// }

int main(int argc, char const *argv[])
{
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
