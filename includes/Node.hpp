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

        void                        setLeft(Node value);
        void                        setRight(Node value);
        void                        setErrorMessage(std::string const &message);

        Node*                       getLeft(void)const;
        Node*                       getRight(void)const;
        std::string                 getType(void)const;
        std::string                 getErrorMessage(void)const;
        std::vector<std::string>&   getContent(void);

        Node*   createNode(std::string const &type);
        Node*   createNodeEmptyLocation(std::string const &type);
        int     parseServer(std::vector<std::string>::iterator &it, std::vector<std::string>::iterator &ite);
        int     parseLocation(std::vector<std::string>::iterator &it, std::vector<std::string>::iterator &ite);
        void    displayContent(std::ostream &o)const;
        void    postfixFree(void);

        static directive_t  initMap(void);
        static bool         isDirectiveLocation(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite, std::string &type);
        static bool         isDirectiveServer(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite, std::string &type);
        static bool         checkDirectiveFormat(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite, std::string const &directive);
        static bool         checkListen(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite);
        static bool         checkIpAndPort(std::vector<std::string> serverInfo);
        static bool         checkIp(std::string ip);
        static bool         checkPort(std::string port);
        static bool         checkServerName(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite);
        static bool         checkMaxBodySize(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite);
        static bool         checkCgi(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite);
        static bool         checkErrorPage(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite);
        static bool         checkMethods(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite);
        static bool         checkIndex(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite);
        static bool         checkRoot(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite);
        static bool         checkRedirect(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite);
        static bool         checkAutoindex(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite);
        static bool         checkUpload(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite);
        static bool         checkLocation(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite);
        static bool         isLocation(std::vector<std::string>::iterator it, std::vector<std::string>::iterator &ite);
        static bool         isNumber(std::string string);

    private:
        Node*                       m_left;
        Node*                       m_right;
        std::string                 m_type;
        std::vector<std::string>    m_content;

        static directive_t          directivesMap;
        static std::string          errorMessage;
        

};

std::ostream&   operator<<(std::ostream &o, Node const &i);


#endif
/* ********************************************************** NODE_HPP */