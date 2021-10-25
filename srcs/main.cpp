#include "Cluster.hpp"
#include "Tree.hpp"

void displayContent(const Node* node)
{
    std::vector<std::string>    tmpContent = node->getContent();
    std::vector<std::string>::const_iterator    it = tmpContent.begin();
    std::vector<std::string>::const_iterator    ite = tmpContent.end();

    std::cout << "(";
    for (; it != ite; it++)
    {
        std::cout << *it;
        if ((it + 1) != ite)
            std::cout << " ,";
    }
    std::cout << ")";
}

void printBT(const std::string& prefix, const Node* node, bool isLeft)
{
    if( node != nullptr )
    {
        std::cout << prefix;

        std::cout << (isLeft ? "├──" : "└──" );

        // print the value of the node
        std::cout << node->getType();
        displayContent(node);
        std::cout << std::endl;
        // enter the next tree level - left and right branch
        printBT( prefix + (isLeft ? "│   " : "    "), node->getRight(), true);
        printBT( prefix + (isLeft ? "│   " : "    "), node->getLeft(), false);
    }
}

void printBT(const Node* node)
{
    printBT("", node, false);    
}

int main(int argc, char const *argv[])
{
    Cluster test;

    // test.run();
    std::cout << test << std::endl;
//----------------------------------------------------------------------------

    // std::cout << "--------PARSING TESTS--------" << std::endl << std::endl;
    // Tree   tree("confFiles/nginx.conf");


    // // tree.parseCluster();
    // std::cout << tree.getRoot() << std::endl << std::endl

    return 0;
}
