#ifndef TOKENIZER_HPP
# define TOKENIZER_HPP

# include "Headers.hpp"

class Tokenizer
{
    public:
        Tokenizer();
        Tokenizer(const char * configFilePath);
        Tokenizer(Tokenizer const &src);
        ~Tokenizer();

        Tokenizer&     operator=(Tokenizer const &rhs);

        void    openConfigFile(void);
        void    fillBufferStr(void);
        void    searchAndReplace(std::string const &str, std::string const &toReplace);
        void    replaceAllSeparators(void);
        void    createTokensList(void);
        void    displayList(std::ostream & o)const;

        const char *                getConfigFilePath(void) const;
        std::string                 getBufferStr(void) const;
        std::vector<std::string>    getTokens(void) const;

    private:
        const char *                m_configFilePath;
        std::string                 m_bufferStr;
        std::ifstream               m_ifs;
        std::vector<std::string>    m_list;

        class nullPathException : public std::exception
        {
            public: 
                virtual const char* what() const throw();
        };
        class wrongPathException : public std::exception
        {
            public: 
                virtual const char* what() const throw();
        };
        class isNotReadableException : public std::exception
        {
            public: 
                virtual const char* what() const throw();
        };
        class isNotRegulatException : public std::exception
        {
            public: 
                virtual const char* what() const throw();
        };
        class canNotOpenException : public std::exception
        {
            public: 
                virtual const char* what() const throw();
        };
};

std::ostream&   operator<<(std::ostream &o, Tokenizer const &i);

#endif
/* ********************************************************** TOKENIZER_HPP */