#include "Client.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Client::Client()
{
}

Client::Client( const Client & src )
{
}

Client::Client(fd_type client_fd, address_type & theirAddr, fd_type epoll): ASocket()
{
	m_fd = client_fd;
	m_addr = theirAddr;
	_makeFdNonBlocking();
	_epollCtlAdd(epoll);
	m_list.insert(this);
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
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Client const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

void                   Client::sendResponse(char const *response) const
{
	std:: cout << m_fd << std::endl;
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