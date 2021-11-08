#include "FileDescriptor.hpp"

int	FileDescriptor::_epoll_fd = -1;

void	FileDescriptor::setEpollFd(void)
{
	_epoll_fd = epoll_create1(0);
	if(_epoll_fd < 0)
	{
		std::cerr << strerror(errno) << "    " << "Failed to create epoll file descriptor\n";
		return ;
	}
}

int const & FileDescriptor::getEpollFd(void)
{
    return _epoll_fd;
}

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

FileDescriptor::FileDescriptor(void)
: m_fd(-1)
{
}

FileDescriptor::FileDescriptor(const FileDescriptor & src)
: m_fd(src.m_fd)
{
}

FileDescriptor::FileDescriptor(int const fd)
: m_fd(fd)
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

FileDescriptor::~FileDescriptor()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

/*
** --------------------------------- METHODS ----------------------------------
*/


void	FileDescriptor::epollCtlAdd(void)
{
	FileDescriptor::event_type	event;

    memset(&event, 0, sizeof(event));
    event.data.fd = m_fd;
    event.events = EPOLLIN | EPOLLET;
    if(epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, event.data.fd, &event))
    {
        fprintf(stderr, "Failed to add file descriptor to epoll\n");
        close(_epoll_fd);
    }
}

void	FileDescriptor::epollCtlAdd_w(void)
{
	FileDescriptor::event_type	event;

    memset(&event, 0, sizeof(event));
    event.data.fd = m_fd;
    event.events = EPOLLOUT | EPOLLET;
    if(epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, event.data.fd, &event))
    {
        fprintf(stderr, "Failed to add file descriptor to epoll\n");
        close(_epoll_fd);
    }
}


void	FileDescriptor::epollCtlDel(void)
{
	FileDescriptor::event_type	event;

    memset(&event, 0, sizeof(event));
    event.data.fd = m_fd;
    event.events = EPOLLIN | EPOLLET;
    if(epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, event.data.fd, &event))
    {
        fprintf(stderr, "Failed to add file descriptor to epoll\n");
        close(_epoll_fd);
    }
}

void			FileDescriptor::makeFdNonBlocking(void)
{
	if (fcntl(m_fd, F_SETFL, O_NONBLOCK) == -1)
	{
		perror("fcntl");
		exit(1);
	}
    
}
/*
** --------------------------------- ACCESSOR ---------------------------------
*/

int		FileDescriptor::value(void) const
{
	return (m_fd);
}

/* ************************************************************************** */