#include "Cluster.hpp"
#include "Tree.hpp"

void printBT(const std::string& prefix, const Node* node, bool isLeft)
{
    if( node != nullptr )
    {
        std::cout << prefix;

        std::cout << (isLeft ? "├──" : "└──" );

        // print the value of the node
        std::cout << node->getType() << std::endl;

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

    test.run();

    /*std::cout << "--------PARSING TESTS--------" << std::endl << std::endl;
    Tree   tree("test.conf");


    tree.parseCluster();
    // std::cout << tree;
    // std::cout << std::endl << std::endl;
    // printBT(tree.getRoot());
    std::vector<std::string>::iterator it;
    std::vector<std::string>::iterator ite;


    std::cout << tree.getRoot()->directivesMap["server_name"](it, ite) << std::endl;
    return 0;*/
}
