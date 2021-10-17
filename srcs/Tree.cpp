#include "Tree.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/
Tree::Tree() : m_root(NULL)
{}

Tree::Tree(const char * configFilePath) : m_root(NULL), m_tokens(configFilePath)
{}

Tree::Tree(Tree const &src) : m_root(src.m_root), m_tokens(src.m_tokens)
{}
// WE SHOULD TRY IF COPY CONSTRUCTOR IS WORKING PROPERLY!

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/
Tree::~Tree()
{
    if (m_root != NULL)
        delete m_root;
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/
// DEEP COPY ?
Tree& Tree::operator=(Tree const & rhs)
{
    if (this != &rhs)
    {
        this->m_root = rhs.m_root;
        this->m_tokens = rhs.m_tokens;
    }
    return *this;
}

std::ostream&   operator<<(std::ostream &o, Tree const &i)
{
    o << "ROOT:" << std::endl << i.getRoot() << std::endl << std::endl;
    o << "TOKENS: " << std::endl << i.getTokens() << std::endl;
    return o;
}

/*
** --------------------------------- MEMBER FUNCTIONS ----------------------------------
*/
void    Tree::parseCluster(void)
{
    std::vector<std::string>            tokens = m_tokens.getTokens();
    std::vector<std::string>::iterator  it = tokens.begin();
    std::vector<std::string>::iterator  ite = tokens.end();

    Node*   currentNode = m_root;

    while(it != ite)
    {
        if (this->isServer(it, ite) == true) // STRANGE FUNCITON MEMBER ?
            {
                currentNode = this->createServerNode(currentNode); // NODE FUNCTION MEMBER ??
                // this->parseServer(currentNode, it, ite);
            }
        else
            std::cout << "ERROR ?" << std::endl;
    }
}

bool    Tree::isServer(std::vector<std::string>::iterator &it, std::vector<std::string>::iterator &ite)
{
    if (*it == "server" && ++it != ite && *it == "{")
        return true;
    return false;
}

Node*    Tree::createServerNode(Node *currentNode)
{
    if (currentNode == NULL)
        return new Node("Server");
    else if (currentNode->getLeft() == NULL)
    {
        currentNode->setLeft(Node("Server"));
        return currentNode->getLeft();
    }
    else
    {
        currentNode->setRight(Node("Server"));
        return currentNode->getRight();
    }
}

// void    Tree::parseServer(Node *currentNode, std::vector<std::string>::iterator &it, std::vector<std::string>::iterator &ite)
// {
//     while (it != ite)
//     {
//         if (*it.isAKeyword())
//             createKeyWordNode(it)
//     }
// }



/*
** --------------------------------- ACCESSOR ---------------------------------
*/
Node*       Tree::getRoot(void) const
{
    return m_root;
}

Tokenizer   Tree::getTokens(void) const
{
    return m_tokens;
}
/*
** --------------------------------- EXCEPTIONS ---------------------------------
*/


/* ************************************************************************** */