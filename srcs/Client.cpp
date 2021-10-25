#include "Client.hpp"
Client::list_type Client::_list = list_type();

void	Client::closeConnexion(Client const & client)
{
	close(client.getFd());
	
    _list.erase(client);
}

Client	const *	Client::getClientFromFd(fd_type fd)
{
	for (list_type::iterator it = _list.begin(); it != _list.end(); it++)
	{
		if ((*it).m_fd == fd)
			return (&(*it));
	}
	return (NULL);
}
/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Client::Client()
{
}

Client::Client( const Client & src ): ASocket(src)
{
	//_list = src._list;
}

Client::Client(fd_type client_fd, address_type & theirAddr, fd_type epoll): Client()
{
	m_fd = client_fd;
	m_addr = theirAddr;
	_makeFdNonBlocking();
	_epollCtlAdd(epoll);
	_list.insert(*this);
}



/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Client::~Client()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Client &				Client::operator=( Client const & rhs )
{
	if ( this != &rhs )
	{
        ASocket::operator=(rhs);
		this->_list = rhs._list;
	}
	return *this;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

void                   Client::sendResponse(char const *response) const
{
    if((send(m_fd, response, strlen(response), 0)) == -1)
	{
		perror ("send");
		exit(1);
	}
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/



/* ************************************************************************** */