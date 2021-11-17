#ifndef ASOCKET_HPP
# define ASOCKET_HPP

# include "Headers.hpp"
# include "FileDescriptor.hpp"
# include "Server.hpp"

class ACgi;

class ASocket
{
	public:
		typedef FileDescriptor				fd_type;
		typedef std::map<int, ASocket *>	list_type;
		typedef struct sockaddr_in			address_type;
		typedef struct epoll_event			event_type;
		typedef	std::string					port_type;
		typedef	std::string					ip_type;

		ASocket();
		ASocket(ASocket const & src);
		ASocket(int const & fd, Server const * server);
		virtual ~ASocket();
		
		int				getFd(void) const;
		virtual bool	execute(ASocket **ptr) = 0;
		virtual bool	send_rep(ASocket ** ptr) = 0;
		virtual bool	alive(void) const = 0;
		void			resetClock(void);
		
		static ASocket *	getSocket(int const & fd);
		static ACgi *		getCgi(int const & fd);
		static void			addSocket(ASocket * socket);
		static void			removeSocket(ASocket * socket);
		static void			removeCgi(ACgi * cgi);
		static void			clean(void);
		static void			clear(void);

	protected:
		fd_type			m_fd;
		Server	const * m_server;
		time_t			m_clock;

	private:
		static	list_type	_list;
		
};

#endif /* ********************************************************* ASOCKET_H */