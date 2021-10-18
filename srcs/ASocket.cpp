#include "ASocket.hpp"


/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ASocket::ASocket()
{

}

ASocket::ASocket( const ASocket & src ): m_fd(src.m_fd), m_addr(src.m_addr), m_event(src.m_event)
{
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
	}
	return *this;
}

bool                    operator<(ASocket & lhs, ASocket & rhs)
{
	return lhs.getFd() < rhs.getFd();
}

bool                    operator<=(ASocket & lhs, ASocket & rhs)
{
	return lhs.getFd() <= rhs.getFd();
}

bool                    operator>(ASocket & lhs, ASocket & rhs)
{
	return lhs.getFd() > rhs.getFd();
}

bool                    operator>=(ASocket & lhs, ASocket & rhs)
{
	return lhs.getFd() >= rhs.getFd();
}

bool                    operator==(ASocket const & lhs, ASocket const & rhs)
{
	return lhs.getFd() == rhs.getFd();
}

bool                    operator!=(ASocket & lhs, ASocket & rhs)
{
	return lhs.getFd() != rhs.getFd();
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
	int s;
	s = fcntl (m_fd, F_SETFL, O_NONBLOCK);
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
        return ;
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