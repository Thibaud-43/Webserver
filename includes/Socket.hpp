#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <iostream>
# include <string>
# include <set>
# include <fcntl.h>
# include "Server.hpp"


enum    socket_type
{
    listener,
    client,
};

class Socket
{
	public:
        typedef std::set<Socket>       list_type;
        typedef struct sockaddr_in     address_type;
        typedef int                    fd_type;

        Socket(Server * server, socket_type type);
    	Socket(Socket const & src);
		~Socket();

		Socket &		               operator=( Socket const & rhs );
        static Socket const *          getSocketFromFd(fd_type fd);
        Server const *                 getServer(void);
        Server const *                 getType(void);
        Server const *                 getFd(void);
        Server const *                 getAddr(void);
        Server const *                 getList(void);
        static  void                   closeFd(fd_type fd);
        void                           sendResponse(char *response);

        operator                       fd_type(void) const;
        friend bool                    operator<(Socket & lhs, Socket & rhs);
        friend bool                    operator<=(Socket & lhs, Socket & rhs);
        friend bool                    operator>(Socket & lhs, Socket & rhs);
        friend bool                    operator>=(Socket & lhs, Socket & rhs);
        friend bool                    operator==(Socket & lhs, Socket & rhs);
        friend bool                    operator!=(Socket & lhs, Socket & rhs);

	private:
		Socket();
        Server const                    *m_server;
        socket_type                     m_type;
        fd_type                         m_fd;
		address_type	                m_addr;
        static list_type                m_list;

        void                            _create(void);
        void                            _bind(void);
        void                            _initAddr(std::string const & port, std::string const & ip);
        void                            _listen(void);
        void                            _makeFdNonBlocking(void);
};


#endif /* ********************************************************* Socket_H */