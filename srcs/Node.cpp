#include "Node.hpp"

Node::directive_t   Node::directivesMap = initMap();

Node::directive_t Node::initMap(void)
{
    directive_t directivesMap;
    {"listen", "server_name", "client_max_body_size", "cgi", "error_page", "methods", "index", "root", "redirect", "autoindex", "upload", "location"};

    directivesMap["listen"] = &Node::checkListen;
    directivesMap["server_name"] = &Node::checkServerName;
    directivesMap["client_max_body_size"] = &Node::checkMaxBodySize;
    directivesMap["cgi"] = &Node::checkCgi;
    directivesMap["error_page"] = &Node::checkErrorPage;
    directivesMap["methods"] = &Node::checkMethods;
    directivesMap["index"] = &Node::checkIndex;
    directivesMap["root"] = &Node::checkRoot;
    directivesMap["redirect"] = &Node::checkRedirect;
    directivesMap["autoindex"] = &Node::checkAutoindex;
    directivesMap["upload"] = &Node::checkUpload;
    directivesMap["location"] = &Node::checkLocation;
    return directivesMap;
}
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
    o << "Obj addrr : " << &i << std::endl;
    o << "Type      : " << i.getType() << std::endl;

    o << "Content   : ";
    i.displayContent(o);
    o << std::endl;

    o << "Left addr : " << i.getLeft() << std::endl;
    o << "Right addr: " << i.getRight() << std::endl;
    
    o << std::endl;

    if (i.getLeft() != NULL)
    {
        o << &i << " left branch:" << std::endl;
        o << *i.getLeft();
    }
    if (i.getRight() != NULL)
    {
        o << &i << " right branch:" << std::endl;
        o << *i.getRight();
    }
    return o;
}


/*
** --------------------------------- MEMBER FUNCTIONS ----------------------------------
*/

Node*   Node::parseServer(std::vector<std::string>::iterator &it, std::vector<std::string>::iterator &ite)
{
    Node*       tmpNode = this;
    std::string type;

    while (it != ite && *it != "}")
    {
        if (Node::isDirectiveServer(it, ite, type) == true) // CHECK the FORMAT // No ref
        {
            tmpNode = tmpNode->createNode(type);
            while (it != ite && *it != ";")
                it++;
            if (*it == ";")
                it++;
            else
                return NULL;
            // tmpNode->fillNode(type, it);
        }
        else
            return NULL;
    }
    if (*it == "}")
        ++it;
    else
        return NULL;
    return tmpNode;
}

Node*   Node::createNode(std::string const &type)
{
    if (this->getLeft() == NULL)
    {
        this->setLeft(Node(type));
        return this->getLeft();
    }
    else
    {
        this->setRight(Node(type));
        return this->getRight();
    }
}

void    Node::displayContent(std::ostream &o)const
{
    std::vector<std::string>::const_iterator  it;
    std::vector<std::string>::const_iterator  ite = m_content.end();

    for (it = m_content.begin(); it != ite; ++it)
    {
        o << *it << " - ";
    }
}

bool    Node::isDirectiveServer(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite, std::string &type)
{
    std::string const directives [12] = {"listen", "server_name", "client_max_body_size", "cgi", "error_page", "methods", "index", "root", "redirect", "autoindex", "upload", "location"};

    if (it != ite)
    {
        for (size_t i = 0; i < 12; i++)
        {
            if (*it == directives[i])
            {
                type = directives[i];
                return true; // return Node::checkDirectiveFormat
            }
        }
    }
    return false;
}

bool    Node::checkListen(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite)
{
    if (++it == ite)
        return false;

    std::string                 buffer;
    std::istringstream          iss(*it);
    std::vector<std::string>    serverInfo; // IP and PORT
    
    while (getline(iss, buffer, ':'))
        if (buffer.empty() == false)
            serverInfo.push_back(buffer);
        return false;
    

    return true;
}

bool    Node::checkIpAndPort(std::vector<std::string> serverInfo)
{
    std::vector<std::string>::iterator  it = serverInfo.begin();
    std::vector<std::string>::iterator  ite = serverInfo.end();

    if (serverInfo.size() < 1 || serverInfo.size() > 2)
        return false;
    if (Node::checkIp(*it) == false)
        return false;
    if (++it != ite && Node::checkPort(*it) == false)
        return false;
    return true;
}

bool    Node::checkIp(std::string ip)
{

}

bool    Node::checkPort(std::string port)
{
    
}

bool    Node::checkServerName(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite)
{
    std::cout << "server_name !" << std::endl;
    return true;
}

bool    Node::checkMaxBodySize(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite)
{

}

bool    Node::checkServerName(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite)
{

}

bool    Node::checkCgi(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite)
{

}

bool    Node::checkErrorPage(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite)
{

}

bool    Node::checkMethods(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite)
{

}

bool    Node::checkIndex(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite)
{

}

bool    Node::checkRoot(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite)
{

}

bool    Node::checkRedirect(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite)
{

}

bool    Node::checkAutoindex(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite)
{

}

bool    Node::checkUpload(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite)
{

}

bool    Node::checkLocation(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite)
{

}


// bool    Node::checkDirectiveFormat(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite, std::string const &directive)
// {
//     std::map<std::string, fcn_t> directivesMap;


//     directivesMap[directive](it, ite);
// }



void    Node::postfixFree(void)
{
    if (this->m_left != NULL)
        this->m_left->postfixFree();
    if (this->m_right != NULL)
        this->m_right->postfixFree();
    delete this;
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