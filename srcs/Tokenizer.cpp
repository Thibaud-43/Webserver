#include "Tokenizer.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/
Tokenizer::Tokenizer() : m_configFilePath(NULL)
{}

// Is it the same ? 
// Tokenizer::Tokenizer()
// {}

Tokenizer::Tokenizer(const char *configFilePath) : m_configFilePath(configFilePath)
{}

Tokenizer::Tokenizer(Tokenizer const &src)
{
    *this = src;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/
Tokenizer::~Tokenizer()
{}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/
Tokenizer&     Tokenizer::operator=(Tokenizer const &rhs)
{
    if (this != &rhs)
    {
        this->m_list = rhs.m_list;
    }
    return *this;
}

std::ostream&   operator<<(std::ostream &o, Tokenizer const &i)
{
    o << "Configuration file path: " << i.getConfigFilePath() << std::endl;
    o << "Use print member function to show tokens list." << std::endl;
    return o;
}

/*
** --------------------------------- MEMBER FUNCTIONS ----------------------------------
*/

void     Tokenizer::openConfigFile(void)
{
    if (!m_configFilePath)
        throw Tokenizer::nullPathException();
    m_ifs.open(m_configFilePath);
}

void     Tokenizer::fillBufferStr(void)
{
    try
    {
        this->openConfigFile();
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error - ";
        std::cerr << e.what();
    }
    
    std::string tmpString;

    while (getline(m_ifs, tmpString))
    {
        m_bufferStr += tmpString;
        m_bufferStr += " ";
    }
    m_ifs.close();
}

void    Tokenizer::searchAndReplace(std::string const &toSearch, std::string const &toReplace)
{
    size_t      pos = m_bufferStr.find(toSearch);

    while (pos != std::string::npos)
    {
        m_bufferStr.replace(pos, toSearch.length(), toReplace);
        pos = m_bufferStr.find(toSearch, pos + toReplace.length());
    }
}

void    Tokenizer::replaceAllSeparators(void)
{
    this->fillBufferStr();
    this->searchAndReplace("{", " { ");
    this->searchAndReplace("}", " } ");
    this->searchAndReplace(";", " ; ");
    this->searchAndReplace("\t", " ");
}

void    Tokenizer::createTokensList(void)
{
    this->replaceAllSeparators();
    std::istringstream  iss(m_bufferStr);
    std::string         token;

    while (getline(iss, token, ' '))
    {
        if (token.empty() == false)
            m_list.push_back(token);
    }
}

void    Tokenizer::displayList(void)
{
    std::vector<std::string>::iterator   it;

    for (it = m_list.begin(); it != m_list.end(); it++)
        std::cout << *it << std::endl;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

const char *                Tokenizer::getConfigFilePath(void) const
{
    return this->m_configFilePath;
}

std::vector<std::string>    Tokenizer::getTokens(void) const
{
    return this->m_list;
}

std::string                 Tokenizer::getBufferStr(void) const
{
    return this->m_bufferStr;
}

/*
** --------------------------------- EXCEPTIONS ---------------------------------
*/
const char* Tokenizer::nullPathException::what() const throw()
{
    return ("Empty path");
}

/* ************************************************************************** */
