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
    // Cluster test;

    // test.run();

    // std::cout << "--------PARSING TESTS--------" << std::endl << std::endl;
    // Tree   tree("test.conf");


    // tree.parseCluster();
    // std::cout << tree;
    // std::cout << std::endl << std::endl;
    // printBT(tree.getRoot());

    // Node test;
    std::vector<std::string>            list;
    list.push_back("upload");
    list.push_back("off");
    list.push_back(";");
    list.push_back(";");
    std::vector<std::string>::iterator  it = list.begin();
    std::vector<std::string>::iterator  ite = list.end();

    if (Node::checkUpload(it, ite) == true)
        std::cout << "checkRedirect : True" << std::endl;
    else
        std::cout << "checkRedirect : False" << std::endl;
    // if (Node::checkIp("43.2.3.4") == true)
    //     std::cout << "Port : True" << std::endl;
    // else
    //     std::cout << "Port : False" << std::endl;
    return 0;
}
