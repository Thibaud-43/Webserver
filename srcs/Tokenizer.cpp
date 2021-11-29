#include "Tokenizer.hpp"
#include "File.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/
Tokenizer::Tokenizer() : m_configFilePath(NULL)
{}

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
{
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/
Tokenizer&     Tokenizer::operator=(Tokenizer const &rhs)
{
    if (this != &rhs)
    {
        this->m_configFilePath = rhs.m_configFilePath;
        this->m_bufferStr = rhs.m_bufferStr;
        this->m_list = rhs.m_list;
    }
    return *this;
}

std::ostream&   operator<<(std::ostream &o, Tokenizer const &i)
{
    if (i.getConfigFilePath() == NULL)
        return o;
    o << "Configuration file path: " << i.getConfigFilePath() << std::endl << std::endl;
    o << "Tokens list: " << std::endl;
    i.displayList(o);
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
    if (m_ifs.is_open() == false)
        throw Tokenizer::wrongPathException();
    File    confFile(m_configFilePath);
    if (confFile.is_readable() == false)
        throw Tokenizer::isNotReadableException();
    if (confFile.is_regular() == false)
    {
        m_ifs.close();
        throw Tokenizer::isNotRegulatException();
    }
}

void     Tokenizer::fillBufferStr(void)
{
    this->openConfigFile();
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

void    Tokenizer::displayList(std::ostream &o)const
{
    std::vector<std::string>::const_iterator   it;

    for (it = m_list.begin(); it != m_list.end(); it++)
        o << *it << " // ";
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
    return ("Empty path.");
}

const char* Tokenizer::wrongPathException::what() const throw()
{
    return ("Impossible to open file. Check path and/or rights.");
}

const char* Tokenizer::isNotReadableException::what() const throw()
{
    return ("Impossible to read file. Check rights.");
}

const char* Tokenizer::isNotRegulatException::what() const throw()
{
    return ("Is not a regular file.");
}

const char* Tokenizer::canNotOpenException::what() const throw()
{
    return ("Is not a regular file.");
}


/* ************************************************************************** */
