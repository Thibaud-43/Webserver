#include "Tokenizer.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/
Tokenizer::Tokenizer() : m_ifs(0), m_configFilePath(), m_tokens()
{}

// Is it the same ? 
// Tokenizer::Tokenizer()
// {}

Tokenizer::Tokenizer(std::string const &configFilePath) : m_configFilePath(configFilePath)
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
        this->m_tokens = rhs.m_tokens;
    }
    return *this;
}

std::ostream&   operator<<(std::ostream &o, Tokenizer const &i)
{
    o << "Configuration file path: " << i.getConfigFilePath() << std::endl;
    o << "Use print member function to show tokens list." << std::endl;
}

/*
** --------------------------------- MEMBER FUNCTIONS ----------------------------------
*/
// std::ifstream     openConfigFile(void)
// {
    

//     return
// }

/*
** --------------------------------- ACCESSOR ---------------------------------
*/
std::ifstream const &             Tokenizer::getIfs(void) const
{
    return this->m_ifs;
}

std::string const &         Tokenizer::getConfigFilePath(void) const
{
    return this->m_configFilePath;
}

std::vector<std::string>    Tokenizer::getTokens(void) const
{
    return this->m_tokens;
}

/* ************************************************************************** */
