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
    o << "TREE:" << std::endl << *i.getRoot() << std::endl << std::endl;
    o << "TOKENS: " << std::endl << i.getTokens() << std::endl;
    return o;
}

/*
** --------------------------------- MEMBER FUNCTIONS ----------------------------------
*/
int    Tree::parseCluster(void)
{
    Node*                               tmpNode = m_root;
    std::vector<std::string>            tokens = m_tokens.getTokens();
    std::vector<std::string>::iterator  it = tokens.begin();
    std::vector<std::string>::iterator  ite = tokens.end();

    while(it != ite)
    {
        if (Tree::isServer(it, ite) == true) // STRANGE FUNCITON MEMBER ? Thib told me to make it as a static
        {
            if (tmpNode == NULL)
            {
                m_root = new Node("Server");
                tmpNode = m_root;
            }
            else
                tmpNode = tmpNode->createNode("Server");
            tmpNode->parseServer(it, ite);
        }
        else
        {
            std::cout << "ERROR SERVER" << std::endl;
            return -1;
        }
    }
    return 0;
}

bool    Tree::isServer(std::vector<std::string>::iterator &it, std::vector<std::string>::iterator &ite)
{
    if (*it == "server" && ++it != ite && *it == "{")
    {
        it++;
        return true;
    }
    return false;
}

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