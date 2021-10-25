#ifndef NODE_HPP
# define NODE_HPP

#include "Headers.hpp"

typedef bool (*fcn_t)(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite);

class   Node
{
    public:
        typedef std::map<std::string, fcn_t> directive_t;

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

        Node*   createNode(std::string const &type);
        Node*   parseServer(std::vector<std::string>::iterator &it, std::vector<std::string>::iterator &ite);

        void    displayContent(std::ostream &o)const;

        void    postfixFree(void);

        static directive_t  initMap(void);

        static bool    isDirectiveServer(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite, std::string &type);
        static bool    checkDirectiveFormat(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite, std::string const &directive);
        static bool    checkListen(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite);
        static bool    checkServerName(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite);

        static directive_t directivesMap;

    private:
        Node*                          m_left;
        Node*                          m_right;
        std::string                    m_type;
        std::vector<std::string>       m_content;
        

};

std::ostream&   operator<<(std::ostream &o, Node const &i);


#endif /* ********************************************************** NODE_HPP */