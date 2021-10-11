#include "ASocket.hpp"

ASocket::list_type ASocket::m_list = list_type();

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ASocket::ASocket()
{

}

ASocket::ASocket( const ASocket & src ): m_fd(src.m_fd), m_addr(src.m_addr)
{

}


// CLIENT & LISTENER
/*ASocket::ASocket(Server * server, socket_type type): m_server(server), m_type(type)
{
	if (m_type == listener)
	{
		_initAddr(m_server->getPort(), m_server->getIp());
		_create();
		_bind();
		_listen();
	}
	m_list.insert(*this);
}
*/

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

ASocket::~ASocket()
{
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

ASocket &				ASocket::operator=( ASocket const & rhs )
{
	if ( this != &rhs )
	{
        m_fd = rhs.m_fd;
		m_addr = rhs.m_addr;
        m_list = rhs.m_list;

	}
	return *this;
}

bool                    operator<(ASocket & lhs, ASocket & rhs)
{
	return lhs.m_fd < rhs.m_fd;
}

bool                    operator<=(ASocket & lhs, ASocket & rhs)
{
	return lhs.m_fd <= rhs.m_fd;
}

bool                    operator>(ASocket & lhs, ASocket & rhs)
{
	return lhs.m_fd > rhs.m_fd;
}

bool                    operator>=(ASocket & lhs, ASocket & rhs)
{
	return lhs.m_fd >= rhs.m_fd;
}

bool                    operator==(ASocket & lhs, ASocket & rhs)
{
	return lhs.m_fd == rhs.m_fd;
}

bool                    operator!=(ASocket & lhs, ASocket & rhs)
{
	return lhs.m_fd != rhs.m_fd;
}

ASocket::operator fd_type(void) const
{
	return m_fd;
}

/*
** --------------------------------- METHODS ----------------------------------
*/



void			ASocket::_makeFdNonBlocking(void)
{
	int flags, s;

	flags = fcntl (m_fd, F_GETFL, 0);
	if (flags == -1)
	{
		perror ("fcntl");
		exit(1);

	}
	flags |= O_NONBLOCK;
	s = fcntl (m_fd, F_SETFL, flags);
	if (s == -1)
	{
		perror ("fcntl");
		exit(1);

	}
	return ;
}

void	ASocket::destroy(void)
{
	if(close(m_fd))
    {
        std::cerr << "Failed to close epoll file descriptor" << std::endl;
        return ;
    }
	m_list.erase(this);
	delete (this);
}

void					ASocket::_epollCtlAdd(fd_type epoll)
{
    m_event.events = EPOLLIN | EPOLLET;                                               // The associated file is available for read(2) operations.
    m_event.data.fd = m_fd;
    
    if(epoll_ctl(epoll, EPOLL_CTL_ADD, m_fd, &m_event))                       // add the file descriptor 0 to our epoll instance epoll_fd, EPOLL_CTL_ADD add the poll to the instance
    {
        fprintf(stderr, "Failed to add file descriptor to epoll\n");
        close(epoll);
        return ;
    }
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/

ASocket *       ASocket::getASocketFromFd(fd_type fd)
{
	for (list_type::iterator it = m_list.begin(); it != m_list.end(); it++)
	{
		if ((*it)->m_fd == fd)
			return (*it);
	}
	return (NULL);
}

ASocket::fd_type	ASocket::getFd(void)
{
	return (m_fd);
}

ASocket::address_type	ASocket::getAddr(void)
{
	return (m_addr);
}

/* ************************************************************************** */