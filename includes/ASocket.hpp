#ifndef ASOCKET_HPP
# define ASOCKET_HPP

# include "Headers.hpp"
# include "FileDescriptor.hpp"

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
		ASocket(int const & fd);
		~ASocket();
		
		int				getFd(void) const;
		virtual bool	execute(void) = 0;
		virtual bool	alive(void) const = 0;
		
		static ASocket *	getSocket(int const & fd);
		static void			addSocket(ASocket * socket);
		static void			removeSocket(ASocket * socket);


	protected:
		fd_type		m_fd;
		time_t		m_clock;
		
		void	resetClock(void);

	private:
		static	list_type	_list;
		
};

#endif /* ********************************************************* ASOCKET_H */