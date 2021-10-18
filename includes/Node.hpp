#ifndef NODE_HPP
# define NODE_HPP

#include "Headers.hpp"

class   Node
{
    public:
        Node();
        Node(std::string type);
        Node(std::string type, std::vector<std::string> content);
        Node(Node const & src);
        ~Node();

        Node&   operator=(Node const &rhs);
        
        Node*                       getLeft(void)const;
        Node*                       getRight(void)const;
        void                        setLeft(Node value);
        void                        setRight(Node value);

        std::string                 getType(void)const;
        std::vector<std::string>    getContent(void)const;

        void    displayContent(std::ostream &o)const;

    private:
        Node*                       m_left;
        Node*                       m_right;
        std::string                 m_type;
        std::vector<std::string>    m_content;
};

std::ostream&   operator<<(std::ostream &o, Node const &i);

#endif /* ********************************************************** NODE_HPP */