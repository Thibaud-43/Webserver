#include "Cgi.hpp"
Cgi::list_type Cgi::_list = list_type();

bool		Cgi::isCgiFd(fd_type & fd)
{
    for (list_type::iterator it = _list.begin(); it != _list.end(); it++)
    {
        if (it->m_pipe[1] == fd)
            return (true);
    }
    return (false);
}

void		Cgi::removeCgi(Cgi const & cgi)
{
	for (list_type::iterator it = _list.begin(); it != _list.end(); it++)
	{
		if (cgi.getClient() == (*it).getClient())
		{
			_list.erase(it);
			return ;
		}
	}
}

Cgi	const *	Cgi::getCgiFromFd(fd_type fd)
{
	for (list_type::iterator it = _list.begin(); it != _list.end(); it++)
	{
		if ((*it).m_pipe[1] == fd)
			return (&(*it));
	}
	return (NULL);
}
/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Cgi::Cgi()
{
}

Cgi::Cgi( const Cgi & src )
{

}

Cgi::Cgi(Request const &  Request)
{

}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Cgi::~Cgi()
{

}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Cgi &				Cgi::operator=( Cgi const & rhs )
{
	if ( this != &rhs )
	{
	}
	return *this;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

bool	Cgi::handle(std::string & buffer) const
{
	return (true);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/
Client const *	Cgi::getClient(void) const
{
	return m_client;
}
/* ************************************************************************** */