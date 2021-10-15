#include "Cluster.hpp"
#include "Tokenizer.hpp"


int main(int argc, char const *argv[])
{
    Cluster test;

    test.run();

    /*std::cout << "PARSING TESTS" << std::endl;
    Tokenizer   test1("test.conf");
    Tokenizer   test2;

    std::cout << "Test1" << std::endl;
    test1.createTokensList();
    test1.displayList();

    // std::cout << std::endl << "Test2" << std::endl;
    // test2.createTokensList();
    // test2.displayList();
    */
    return 0;
}
