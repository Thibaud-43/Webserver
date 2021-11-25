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

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/
Tree::~Tree()
{
    if (m_root != NULL)
        m_root->postfixFree();
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/
Tree& Tree::operator=(Tree const & rhs)
{
    if (this != &rhs)
    {
        this->m_root = rhs.m_root;
        this->m_tokens = rhs.m_tokens;
    }
    return *this;
}

std::ostream&   operator<<(std::ostream &o, Tree &i)
{
    if (i.getRoot() != NULL)
        o << "TREE:" << std::endl << *i.getRoot() << std::endl << std::endl;
    o << "TOKENS: " << std::endl << i.getTokens() << std::endl;
    return o;
}

/*
** --------------------------------- MEMBER FUNCTIONS ----------------------------------
*/
void printBT(const Node* node);

void    Tree::parseTokensList(void)
{
    if (this->parseCluster() < 0)
        throw Tree::ParserFailException();
    if (this->getTokens().getTokens().size() == 0)
    {
        m_root->setErrorMessage("Empty file.");
        throw Tree::ParserFailException();
    }
    // printBT(m_root);
}

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
            if (it != ite && *it == "}")
            {
                if (tmpNode == NULL)
                {
                    m_root = new Node("Server");
                    m_root->getContent().push_back("empty");
                    tmpNode = m_root;
                }
                else
                {
                    tmpNode = tmpNode->createNode("Server");
                    tmpNode->getContent().push_back("empty");
                }
                it++;
            }
            else if (tmpNode == NULL)
            {
                m_root = new Node("Server");
                tmpNode = m_root;
                if (tmpNode->parseServer(it, ite) < 0)
                    return -1;
            }
            else
            {
                tmpNode = tmpNode->createNode("Server");
                if (tmpNode->parseServer(it, ite) < 0)
                    return -1;
            }
        }
        else
        {
            if (m_root->getErrorMessage().empty() == true)
                m_root->setErrorMessage("server scope is wrong.");
            return -1;
        }
    }
    return 0;
}

bool    Tree::isServer(std::vector<std::string>::iterator &it, std::vector<std::string>::iterator &ite)
{
    if (*it == "server" && ++it != ite && *it == "{")
    {
        ++it;
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

Tokenizer&  Tree::getTokens(void)
{
    return m_tokens;
}
/*
** --------------------------------- EXCEPTIONS ---------------------------------
*/

const char* Tree::ParserFailException::what() const throw()
{
    return ("Parsing error - ");
}

/* ************************************************************************** */