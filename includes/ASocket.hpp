#ifndef ASOCKET_HPP
# define ASOCKET_HPP

#include "Headers.hpp"

class Server;
class ASocket
{
	public:
        typedef struct sockaddr_in		address_type;
        typedef int						fd_type;
        typedef struct epoll_event      event_type;
        typedef	std::string				port_type;
        typedef	std::string				ip_type;

		ASocket();
    	ASocket(ASocket const & src);
		virtual ~ASocket() = 0;

		ASocket &				operator=(ASocket const & rhs);
        Server const *			getServer(void) const;
        fd_type					getFd(void) const;
        address_type			getAddr(void) const;
        void					destroy(void);

        operator				fd_type(void) const;
        friend bool				operator<(ASocket & lhs, ASocket & rhs);
        friend bool				operator<=(ASocket & lhs, ASocket & rhs);
        friend bool				operator>(ASocket & lhs, ASocket & rhs);
        friend bool				operator>=(ASocket & lhs, ASocket & rhs);
        friend bool				operator==(ASocket & lhs, ASocket & rhs);
        friend bool				operator!=(ASocket & lhs, ASocket & rhs);

	protected:
        fd_type					m_fd;
		address_type			m_addr;
        event_type              m_event;

        void					_makeFdNonBlocking(void);
        void					_epollCtlAdd(fd_type epoll);

};
#endif /* ********************************************************* ASocket_H */