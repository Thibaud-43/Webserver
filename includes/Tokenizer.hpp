#ifndef TOKENIZER_HPP
# define TOKENIZER_HPP

# include "Headers.hpp"

class Tokenizer
{
    public:
        Tokenizer();
        Tokenizer(std::string );
        Tokenizer(Tokenizer const &src);
        ~Tokenizer();

        Tokenizer&     operator=(Tokenizer const &rhs);

    private:
        int                         m_fd;
        std::string                 m_configFilePath; // DEFAULT = empty
        std::vector<std::string>    m_tokens;
};

std::ostream&   operator<<(std::ostream &o, Tokenizer const &i);

#endif /* ********************************************************** TOKENIZER_HPP */