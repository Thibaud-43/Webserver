#include "ASocket.hpp"

ASocket::fd_type	ASocket::_epoll = 0;

ASocket::fd_type const &	ASocket::getEpoll(void)
{
	return (_epoll);
}

void		ASocket::setEpoll(fd_type const & epoll)
{
	_epoll = epoll;
}

void	ASocket::epollCtlAdd(ASocket::fd_type const & fd)
{
	ASocket::event_type	event;

    memset(&event, 0, sizeof(event));
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    if(epoll_ctl(ASocket::getEpoll(), EPOLL_CTL_ADD, event.data.fd, &event))
    {
        fprintf(stderr, "Failed to add file descriptor to epoll\n");
        close(ASocket::getEpoll());
    }
}

void	ASocket::epollCtlDel(ASocket::fd_type const & fd)
{
	ASocket::event_type	event;

    memset(&event, 0, sizeof(event));
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    if(epoll_ctl(ASocket::getEpoll(), EPOLL_CTL_DEL, event.data.fd, &event))
    {
        fprintf(stderr, "Failed to add file descriptor to epoll\n");
        close(ASocket::getEpoll());
    }
}
/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ASocket::ASocket()
{

}

ASocket::ASocket( const ASocket & src ): m_fd(src.m_fd), m_addr(src.m_addr)
{
	m_event.data.fd = src.m_event.data.fd;
	m_event.data.ptr = src.m_event.data.ptr;
	m_event.data.u32 = src.m_event.data.u32;
	m_event.data.u64 = src.m_event.data.u64;
	m_event.events = src.m_event.events;
}

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
        m_fd = rhs.getFd();
		m_addr = rhs.m_addr;
		m_event.data.fd = rhs.m_event.data.fd;
		m_event.data.ptr = rhs.m_event.data.ptr;
		m_event.data.u32 = rhs.m_event.data.u32;
		m_event.data.u64 = rhs.m_event.data.u64;
		m_event.events = rhs.m_event.events;
	}
	return *this;
}

bool                    operator<(ASocket const & lhs, ASocket const & rhs)
{
	return lhs.m_fd < rhs.m_fd;
}

bool                    operator<=(ASocket const & lhs, ASocket const & rhs)
{
	return lhs.m_fd <= rhs.m_fd;
}

bool                    operator>(ASocket const & lhs, ASocket const & rhs)
{
	return lhs.m_fd > rhs.m_fd;
}

bool                    operator>=(ASocket const & lhs, ASocket const & rhs)
{
	return lhs.m_fd >= rhs.m_fd;
}

bool                    operator==(ASocket const & lhs, ASocket const & rhs)
{
	return lhs.m_fd == rhs.m_fd;
}

bool                    operator!=(ASocket const & lhs, ASocket const & rhs)
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
	if (fcntl(m_fd, F_SETFL, O_NONBLOCK) == -1)
	{
		perror("fcntl");
		exit(1);
	}
}

void	ASocket::destroy(void)
{
	if(close(m_fd))
        std::cerr << "Failed to close epoll file descriptor" << std::endl;
}

void					ASocket::_epollCtlAdd(fd_type & epoll)
{
    memset(&m_event, 0, sizeof(m_event));
    m_event.events = EPOLLIN | EPOLLET;                                               // The associated file is available for read(2) operations.
    m_event.data.fd = m_fd;
    
    if(epoll_ctl(epoll, EPOLL_CTL_ADD, m_fd, &m_event))                       // add the file descriptor 0 to our epoll instance epoll_fd, EPOLL_CTL_ADD add the poll to the instance
    {
        fprintf(stderr, "Failed to add file descriptor to epoll\n");
        close(epoll);
    }
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

ASocket::fd_type	ASocket::getFd(void) const
{
	return (m_fd);
}

ASocket::address_type	ASocket::getAddr(void) const
{
	return (m_addr);
}

/* ************************************************************************** */