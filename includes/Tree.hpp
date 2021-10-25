#ifndef TREE_HPP
# define TREE_HPP

# include "Headers.hpp"
# include "Node.hpp"
# include "Tokenizer.hpp"

class   Tree
{
    public:
        Tree();
        Tree(const char * configFilePath);
        Tree(Tree const & src);
        ~Tree();

        Tree&   operator=(Tree const &rhs);

        Node*       getRoot(void) const;
        Tokenizer   getTokens(void) const;
        
        void    parseTokensList(void);
        int     parseCluster(void);

        static bool    isServer(std::vector<std::string>::iterator &it, std::vector<std::string>::iterator &ite);

        void    postfixSearch(Node *node);

        // void        displayTokens(void)const;
        // void        displayRoot(void)const;

    private:
        Node*       m_root;
        Tokenizer   m_tokens;
        class TokenizerFailException : public std::exception
        {
            public: 
                virtual const char* what() const throw();
        };
        class   ParserFailException : public std::exception
        {
            public: 
                virtual const char* what() const throw();
        };


};

std::ostream&   operator<<(std::ostream &o, Tree const &i);

#endif /* ********************************************************** TREE_HPP */