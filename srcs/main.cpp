#include "Cluster.hpp"
#include "Tree.hpp"

void printBT(const std::string& prefix, Node* node, bool isLeft)
{


    if( node != NULL )
    {
        std::vector<std::string>::const_iterator it = node->getContent().begin();
	    std::vector<std::string>::const_iterator ite = node->getContent().end();
        std::cout << prefix;

        std::cout << (isLeft ? "├──" : "└──" );

        // print the value of the node
        std::cout << node->getType();
        if (it != ite)
            std::cout << "(" << *it << ")";
        std::cout<< std::endl;

        // enter the next tree level - left and right branch
        printBT( prefix + (isLeft ? "│   " : "    "), node->getRight(), true);
        printBT( prefix + (isLeft ? "│   " : "    "), node->getLeft(), false);
    }
}

void printBT(Node* node)
{
    printBT("", node, false);    
}

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
