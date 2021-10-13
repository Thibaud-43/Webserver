#ifndef TOKENIZER_HPP
# define TOKENIZER_HPP

# include "Headers.hpp"

class Tokenizer
{
    public:
        Tokenizer();
        Tokenizer(std::string const &configFilePath);
        Tokenizer(Tokenizer const &src);
        ~Tokenizer();

        Tokenizer&     operator=(Tokenizer const &rhs);

        int     openConfigFile(void); // NOT CONST BECAUSE CHANGING M_IFS VALUE

        // std::ifstream const &       getIfs(void) const;
        std::string const &         getConfigFilePath(void) const;
        std::vector<std::string>    getTokens(void) const;

    private:
        std::ifstream               m_ifs;
        std::string const           m_configFilePath; // DEFAULT = empty
        std::vector<std::string>    m_tokens;
};

std::ostream&   operator<<(std::ostream &o, Tokenizer const &i);

#endif /* ********************************************************** TOKENIZER_HPP */