#include "Node.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/
Node::Node() : m_left(NULL), m_right(NULL)
{}

Node::Node(std::string type) : m_left(NULL), m_right(NULL), m_type(type)
{}

Node::Node(std::string type, std::vector<std::string> content) : m_left(NULL), m_right(NULL), m_type(type), m_content(content)
{}


Node::Node(Node const &src) : m_left(src.m_left), m_right(src.m_right), m_type(src.m_type), m_content(src.m_content)
{}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/
Node::~Node()
{}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/
Node&     Node::operator=(Node const & rhs)
{
    if (this != &rhs)
    {
        m_left = rhs.m_left;
        m_right = rhs.m_right;
        m_type = rhs.m_type;
        m_content = rhs.m_content;
    }
    return *this;
}

std::ostream&			operator<<(std::ostream & o, Node const &i)
{
    // o << "Left addr : " << i.getLeft() << std::endl;
    // o << "Right addr: " << i.getRight() << std::endl;
    o << "Type      : " << i.getType() << std::endl;
    o << "Content   : ";
    i.displayContent(o);
    return o;
}


/*
** --------------------------------- MEMBER FUNCTIONS ----------------------------------
*/
void    Node::displayContent(std::ostream &o)const
{
    std::vector<std::string>::const_iterator  it;
    std::vector<std::string>::const_iterator  ite = m_content.end();

    for (it = m_content.begin(); it != ite; ++it)
    {
        o << *it << " - ";
    }
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/
Node*                       Node::getLeft(void)const
{
    return m_left;
}

Node*                       Node::getRight(void)const
{
    return m_right;
}

void                        Node::setLeft(Node value)
{
    m_left = new Node(value);
}

void                        Node::setRight(Node value)
{
    m_right = new Node(value);
}

std::string                 Node::getType(void)const
{
    return m_type;
}

std::vector<std::string>    Node::getContent(void)const
{
    return m_content;
}

/*
** --------------------------------- EXCEPTIONS ---------------------------------
*/


/* ************************************************************************** */