#include "Response.hpp"
#include "ASocket.hpp"
#include "ACgi.hpp"

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
		if (it->second != socket)
		{
			delete it->second;
			it->second = socket;
		}
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

void	ASocket::removeCgi(ACgi * cgi)
{
	ASocket							*socket;
	ASocket::list_type::iterator	it;

	if (!(socket = dynamic_cast<ASocket *>(cgi)))
		return ;
	it = _list.find(socket->getFd());
	if (it != _list.end())
	{
		close(it->second->getFd());
		delete it->second;
		_list.erase(it);
	}
}

void	ASocket::clean(void)
{
	ACgi				*cgi = NULL;
	list_type::iterator	del;

	for (list_type::iterator it = _list.begin(); it != _list.end(); it++)
	{
		if (!(it->second->alive()))
		{
			std::cout << "[" << HTTPDate() << "]: Delay over connection with client " << it->second->getFd() << " is closed" << std::endl;
			if ((cgi = dynamic_cast<ACgi *>(it->second)))
				cgi->clear();
			del = it;
			it--;
			close(del->second->getFd());
			delete del->second;
			_list.erase(del);
		}
		else if ((cgi = dynamic_cast<ACgi *>(it->second)) && cgi->getPid() >= 0 && !cgi->checkStatus())
		{
			del = it;
			it--;
			close(del->second->getFd());
			delete del->second;
			_list.erase(del);
		}
	}
}

ACgi *	ASocket::getCgi(int const & fd)
{
	ACgi	*cgi;

	for (list_type::iterator it = _list.begin(); it != _list.end(); it++)
	{
		if ((cgi = dynamic_cast<ACgi *>(it->second)) && (fd == cgi->getFdIn() || fd == cgi->getFdOut()))
			return (cgi);
	}
	return (NULL);
}

void	ASocket::clear()
{
	for (list_type::iterator it = _list.begin(); it != _list.end(); it++)
	{
		close(it->second->getFd());
		delete it->second;
	}
	_list.clear();
}

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ASocket::ASocket()
: m_fd(ASocket::fd_type()), m_server(NULL), m_clock(time(NULL))
{

}

ASocket::ASocket( const ASocket & src )
: m_fd(src.m_fd), m_server(src.m_server), m_clock(src.m_clock)
{
}

ASocket::ASocket( const int & fd, Server const * server)
: m_fd(fd), m_server(server), m_clock(time(NULL))
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