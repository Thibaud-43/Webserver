#ifndef ASOCKET_HPP
# define ASOCKET_HPP

#include "Headers.hpp"

class Server;

class ASocket
{
	public:
		typedef struct sockaddr_in		address_type;
		typedef int						fd_type;
		typedef struct epoll_event		event_type;
		typedef	std::string				port_type;
		typedef	std::string				ip_type;

		ASocket();
		ASocket(ASocket const & src);
		virtual ~ASocket() = 0;

		ASocket &		operator=(ASocket const & rhs);
		fd_type			getFd(void) const;
		address_type	getAddr(void) const;
		static fd_type const &	getEpoll(void);
		static void		setEpoll(fd_type const & epoll);
		void			destroy(void);
		static void		epollCtlAdd(fd_type const & fd);
		static void		epollCtlAdd_w(fd_type const & fd);
		static void		epollCtlDel(fd_type const & fd);
		static void		epollCtlDel_w(fd_type const & fd);

		operator		fd_type(void) const;
		friend bool		operator<(ASocket const & lhs, ASocket const & rhs);
		friend bool		operator<=(ASocket const & lhs, ASocket const & rhs);
		friend bool		operator>(ASocket const & lhs, ASocket const & rhs);
		friend bool		operator>=(ASocket const & lhs, ASocket const & rhs);
		friend bool		operator==(ASocket const & lhs, ASocket const & rhs);
		friend bool		operator!=(ASocket const & lhs, ASocket const & rhs);

	protected:
		fd_type							m_fd;
		address_type					m_addr;
		event_type						m_event;
		static	fd_type					_epoll;
		void							_makeFdNonBlocking(void);
		void							_epollCtlAdd(fd_type & epoll);
};
#endif /* ********************************************************* ASocket_H */