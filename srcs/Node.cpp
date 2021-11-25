#include "Node.hpp"

Node::directive_t   Node::directivesMap = initMap();

std::string         Node::errorMessage = std::string();

Node::directive_t Node::initMap(void)
{
    directive_t directivesMap;

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
{
	// std::cout << "Node destructor called" << std::endl;
}

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

bool    Node::isLocation(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite)
{
    if (*it != "location")
        return false;
    if (++it == ite || *it == ";" || *it == "{" || *it == "}")
        return false;
    if (++it == ite || *it != "{")
        return false;
    if (++it == ite)
        return false;
    return true;
}

int     Node::parseLocation(std::vector<std::string>::iterator &it, std::vector<std::string>::iterator &ite)
{
    Node*       tmpNode = this;
    std::string type;

    for (size_t i = 0; i < 3; ++i)
        it++;
    while (it != ite && *it != "}")
    {
        if (Node::isDirectiveLocation(it, ite, type) == true) // CHECK the FORMAT // No ref
        {
            tmpNode = tmpNode->createNode(type);
            while (*(++it) != ";")
                tmpNode->m_content.push_back(*it);
            it++;
        }
        else
        {
            errorMessage = "wrong directive name";
            return -1;
        }
    }
    if (it != ite && *it == "}")
        ++it;
    else
    {
        errorMessage = "missing curly brackets";
        return -1;
    }
    return 0;
}

int     Node::parseServer(std::vector<std::string>::iterator &it, std::vector<std::string>::iterator &ite)
{
    Node*       tmpNode = this;
    std::string type;

    while (it != ite && *it != "}")
    {
        if (Node::isLocation(it, ite) == true)
        {
            tmpNode = tmpNode->createNode("location");
            tmpNode->m_content.push_back(*(it + 1));
            if (*(it + 3) == "}") // CAREFUL
            {
                tmpNode->createNode("emptyLocation");
                it += 4;
            }
            else
                if (tmpNode->parseLocation(it, ite) < 0)
                    return -1;
        }
        else if (Node::isDirectiveServer(it, ite, type) == true) // CHECK the FORMAT // No ref
        {
            tmpNode = tmpNode->createNode(type);
            while (*(++it) != ";")
                tmpNode->m_content.push_back(*it);
            if (it != ite && *it == ";")
                it++;
        }
        else
        {
            if (it != ite && *it == ";")
                errorMessage = "too much semicolon.";
            if (errorMessage.empty() == true)
                errorMessage = "location scope is wrong.";
            return -1;
        }
    }
    if (it != ite && *it == "}")
        ++it;
    else
    {
        errorMessage = "missing curly brackets";
        return -1;
    }
    return 0;
}

bool    Node::checkDirectiveFormat(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite, std::string const &directive)
{
    Node::directive_t::const_iterator    itMap = directivesMap.begin();
    Node::directive_t::const_iterator    iteMap = directivesMap.end();

    while (itMap != iteMap && itMap->first != directive)
        itMap++;
    if (itMap == iteMap)
        return false;
    return (itMap->second)(it, ite);
}

bool    Node::isDirectiveLocation(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite, std::string &type)
{
    std::string const directives [9] = {"client_max_body_size", "cgi", "error_page", "methods", "index", "root", "redirect", "autoindex", "upload"};

    if (it != ite)
    {
        for (size_t i = 0; i < 9; i++)
        {
            if (*it == directives[i])
            {
                type = directives[i];
                if (Node::checkDirectiveFormat(it, ite, directives[i]) == false)
                {
                    errorMessage = directives[i] + " directive is wrong.";
                    return false;
                }
                else
                    return true;
            }
        }
    }
    return false;
}

bool    Node::isDirectiveServer(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite, std::string &type)
{
    std::string const directives [11] = {"listen", "server_name", "client_max_body_size", "cgi", "error_page", "methods", "index", "root", "redirect", "autoindex", "upload"};

    if (it != ite)
    {
        for (size_t i = 0; i < 11; i++)
        {
            if (*it == directives[i])
            {
                type = directives[i];
                return true;
            }
        }
    }
    errorMessage = *it + " directive is wrong.";
    return false;
}

bool    Node::checkListen(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite)
{
    if (++it == ite)        // token after "listen"
        return false;
    int                         colonNb(0);
    std::string                 buffer;
    std::istringstream          iss(*it);
    std::vector<std::string>    serverInfo; // IP and PORT
    
    if ((*it).at(0) == ':' || (*it).at((*it).length() - 1) == ':')
        return false;
    while (getline(iss, buffer, ':'))
    {
        if (buffer.empty() == false)
            serverInfo.push_back(buffer);
        else
            colonNb++;
    }
    if ((colonNb > 0) || Node::checkIpAndPort(serverInfo) == false)
        return false;
    if (++it == ite || *it != ";")
        return false;
    return true;
}

bool    Node::checkIpAndPort(std::vector<std::string> serverInfo)
{
    std::vector<std::string>::iterator  it = serverInfo.begin();

    if (serverInfo.size() < 1 || serverInfo.size() > 2)
        return false;
    if (serverInfo.size() == 1 && Node::checkIp(*it) == false && Node::checkPort(*it) == false)
        return false;
    if (serverInfo.size() == 2 && (Node::checkIp(*it) == false || Node::checkPort(*(++it)) == false))
        return false;
    return true;
}

bool    Node::isNumber(std::string string)
{
    std::string::const_iterator it = string.begin();
    std::string::const_iterator ite = string.end();

    while(it != ite && std::isdigit(*it))
        it++;
    if (string.empty() == false && it == ite)
        return true;
    return false;
}

bool    Node::checkIp(std::string ip)
{
    int                         periodNb(0);
    int                         ipValue;
    std::string                 buffer;
    std::istringstream          iss(ip);
    std::istringstream          tmpIss;
    
    if (ip.at(0) == '.' || ip.at(ip.length() - 1) == '.')
        return false;
    while (getline(iss, buffer, '.'))
    {
        if (buffer.empty() == false)
        {
            if (Node::isNumber(buffer) == false)
                return false;
            tmpIss.str(buffer);
            tmpIss >> ipValue;
            if (ipValue < 0 || ipValue > 255)
                return false;
            tmpIss.clear();
            
        }
        periodNb++;
    }
    if (periodNb != 4)
        return false;
    return true;
}

bool    Node::checkPort(std::string port)
{
    int                 portValue;
    std::istringstream  tmpIss(port);

    if (Node::isNumber(port) == false)
        return false;
    tmpIss >> portValue;
    if (portValue < 0 || portValue > 65535)
        return false;
    return true;
}

bool    Node::checkServerName(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite)
{
    size_t i = 0; 

    if (++it == ite)
        return false;
    while (it != ite && *it != ";" && *it != "{" && *it != "}" && i++ < 32)
    {
        it++;
        i++;
    }
    if (i >= 32 || it == ite) // MIN MAX NB OF SERVER NAMES
        return false;
    if (*it != ";")
        return false;
    return true;
}

bool    Node::checkMaxBodySize(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite)
{
    std::istringstream  tmpIss;
    float               f;

    if (++it == ite)
        return false;
    if (Node::isNumber(*it) == false || (*it).size() > 10)
        return false;
    tmpIss.str(*it);
    tmpIss >> f;
    if (f > 2147483647.0)
        return false;
    if (++it == ite || *it != ";")
        return false;
    return true;
}

bool    Node::checkCgi(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite)
{
    if (++it == ite)
        return false;
    if (*it != ".php" && *it != ".bla") // ACCEPTED EXTENSIONS
        return false;
    if (++it != ite && *it != ";" && *it != "{" && *it != "}" && ++it != ite && *it == ";")
        return true;
    return false;
}

bool    Node::checkErrorPage(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite)
{
    float               f;
    std::istringstream  tmpIss;

    if (++it == ite)
        return false;
    if (Node::isNumber(*it) == false || (*it).size() > 10)
        return false;
    tmpIss.str(*it);
    tmpIss >> f;
    if (f > 2147483647.0)
        return false;
    if (++it == ite || *it == ";" || *it == "{" || *it == "}" || ++it == ite || *it != ";")
        return false;
    return true;
}

bool    Node::checkMethods(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite)
{
    size_t i = 0;

    if (++it == ite)
        return false;
    while (it != ite && *it != ";" && i < 3)
    {
        if (*it != "GET" && *it != "POST" && *it != "DELETE")
            return false;
        i++;
        it++;
    }
    if (i == 0 || it == ite || *it != ";")
        return false;
    return true;
}

bool    Node::checkIndex(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite)
{
    size_t i = 0; 

    if (++it == ite)
        return false;
    while (it != ite && *it != ";" && *it != "{" && *it != "}" && i++ < 32)
    {
        it++;
        i++;
    }
    if (i >= 32 || it == ite) // MIN MAX NB OF SERVER NAMES
        return false;
    if (*it != ";")
        return false;
    return true;
}

bool    Node::checkRoot(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite)
{
    if (++it == ite || *it == ";" || *it == "{" || *it == "}" || ++it == ite || *it != ";")
        return false;
    return true;
}

bool    Node::checkRedirect(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite)
{
    float               f;
    std::istringstream  tmpIss;

    if (++it == ite)
        return false;
    if (Node::isNumber(*it) == false || (*it).size() > 10)
        return false;
    tmpIss.str(*it);
    tmpIss >> f;
    if (f != 301.0 && f != 302.0 && f != 303.0 && f != 304.0 && f != 306.0 && f != 307.0 && f != 308.0)
            return false;
    if (++it == ite || *it == ";" || *it == "{" || *it == "}" || ++it == ite || *it != ";")
        return false;
    return true;
}

bool    Node::checkAutoindex(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite)
{
    if (++it == ite)
        return false;  
    if (*it != "on" && *it != "off")
        return false;
    if (++it == ite || *it != ";")
        return false;
    return true;
}

bool    Node::checkUpload(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite)
{
    if (++it == ite)
        return false;  
    if (*it != "on" && *it != "off")
        return false;
    if (++it == ite || *it != ";")
        return false;
    return true;
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

void    Node::postfixFree(void)
{
    if (this->m_left != NULL)
        this->m_left->postfixFree();
    if (this->m_right != NULL)
        this->m_right->postfixFree();
    delete this;
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

void                        Node::setErrorMessage(std::string const &message)
{
    errorMessage = message;
}

std::string                 Node::getType(void)const
{
    return m_type;
}

std::string                 Node::getErrorMessage(void)const
{
    return errorMessage;
}

std::vector<std::string>&    Node::getContent(void)
{
    return m_content;
}

// /*
// ** --------------------------------- EXCEPTIONS ---------------------------------
// */

// const char* Node::ParserFailException::what() const throw()
// {
//     return ("Parsing error - Missing curly brackets");
// }

// /* ************************************************************************** */

/* ************************************************************************** */