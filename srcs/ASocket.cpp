#include "ASocket.hpp"

ASocket::list_type ASocket::_list = ASocket::list_type();

ASocket	*	ASocket::getSocket(int const & fd)
{
	ASocket::list_type::iterator	it = _list.find(fd);

	return (it == _list.end() ? NULL : it->second);
}

void	ASocket::addSocket(ASocket * socket)
{
	ASocket::list_type::iterator	it = _list.find(socket->getFd());

	if (it != _list.end())
	{
		delete it->second;
		it->second = socket;
	}
	else
		_list[socket->getFd()] = socket;
}

void	ASocket::removeSocket(ASocket * socket)
{
	ASocket::list_type::iterator	it = _list.find(socket->getFd());

	if (it != _list.end())
	{
		close(it->second->getFd());
		delete it->second;
		_list.erase(it);
	}
}

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ASocket::ASocket(): m_fd(ASocket::fd_type()), m_server(NULL), m_clock(time(NULL))
{

}

ASocket::ASocket( const ASocket & src ): m_fd(src.m_fd), m_server(src.m_server), m_clock(src.m_clock)
{

}

ASocket::ASocket( const int & fd, Server const * server): m_fd(fd), m_server(server), m_clock(time(NULL))
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

/*
** --------------------------------- METHODS ----------------------------------
*/

void	ASocket::resetClock(void)
{
	m_clock = time(NULL);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

int		ASocket::getFd(void) const
{
	return (m_fd.value());
}


/* ************************************************************************** */