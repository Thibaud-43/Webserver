#include "Cgi.hpp"

Cgi::list_type Cgi::_list = Cgi::list_type();

Cgi const	*	Cgi::getCgi(int const & fd)
{
	for (list_type::const_iterator it = _list.begin(); it != _list.end(); it++)
	{
		if (fd == (*it)->getFdIn() || fd == (*it)->getFdOut())
			return (*it);
	}
	return (NULL);
}

Cgi const *	Cgi::addCgi(Cgi * cgi)
{
	return (*_list.insert(cgi).first);
}

void	Cgi::removeCgi(Cgi * cgi)
{
	if (cgi->getFdIn() >= 0)
		close(cgi->getFdIn());
	if (cgi->getFdOut() >= 0)
		close(cgi->getFdOut());
	_list.erase(cgi);
}

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Cgi::Cgi()
: Get(), Post(), m_fd_in(-1), m_fd_out(-1)
{
}

Cgi::Cgi( const Cgi & src )
: Get(src), Post(src), m_fd_in(-1), m_fd_out(-1)
{
}

Cgi::Cgi(Get const & src)
: Post(), Get(src), m_fd_in(-1), m_fd_out(-1)
{
	// STARTING CGI_GET
}

Cgi::Cgi(Post const & src)
: Get(), Post(src), m_fd_in(-1), m_fd_out(-1)
{
	// STARTING CGI_POST
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

/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */