#ifndef TREE_HPP
# define TREE_HPP

#include "Headers.hpp"
#include "Node.hpp"
#include "Tokenizer.hpp"

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

        void    parseCluster(void);
        void    parseServer(Node *currentNode, std::vector<std::string>::iterator &it, std::vector<std::string>::iterator &ite);
        void    parseLocation(Node *currentNode);
        Node*   createServerNode(Node *currentNode);

        static bool    isServer(std::vector<std::string>::iterator &it, std::vector<std::string>::iterator &ite);

        // void        displayTokens(void)const;
        // void        displayRoot(void)const;

    private:
        Node*       m_root;
        Tokenizer   m_tokens;

};

std::ostream&   operator<<(std::ostream &o, Tree const &i);

#endif /* ********************************************************** TREE_HPP */