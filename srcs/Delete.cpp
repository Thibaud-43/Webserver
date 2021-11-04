#include "Delete.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Delete::Delete()
: Request()
{
}

Delete::Delete(Delete const & src)
: Request(src)
{
}

Delete::Delete(Request const & src)
: Request(src)
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Delete::~Delete()
{
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

/*
** --------------------------------- METHODS ----------------------------------
*/

bool	Delete::_check(void) const
{
	if (m_header.find("Content-Length") != m_header.end() || m_header.find("Transfer-encoding") != m_header.end())
	{
		Response::send_error("413", )
		return (false);
	}
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */