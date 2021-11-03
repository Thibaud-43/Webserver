#include "FileDescriptor.hpp"

int	FileDescriptor::_epoll_fd = -1;

void	FileDescriptor::setEpollFd(int const fd)
{
	_epoll_fd = fd;
}

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

FileDescriptor::FileDescriptor(int const fd = -1): m_fd(fd)
{
}

FileDescriptor::FileDescriptor(const FileDescriptor & src): m_fd(src.m_fd)
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

FileDescriptor &				FileDescriptor::operator=( FileDescriptor const & rhs )
{
	if ( this != &rhs )
		m_fd = rhs.m_fd;
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

// AJOUTEZ METHODES


/*
** --------------------------------- ACCESSOR ---------------------------------
*/

int		FileDescriptor::value(void) const
{
	return (m_fd);
}

/* ************************************************************************** */