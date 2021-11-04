#include "Get.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Get::Get()
: Request()
{
}

Get::Get( const Get & src )
: Request(src)
{
}

Get::Get(Request const & src)
: Request(src)
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Get::~Get()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/


/*
** --------------------------------- METHODS ----------------------------------
*/

bool	Get::execute(ASocket ** ptr)
{
	File	file(m_path);

	if (ptr)
		*ptr = this;
	if (!_check())
		return (false);
	if (file.is_directory())
	{
		if (_manageDir())
		{
			_convertToClient(ptr);
			return (true);
		}
		return (false);
	}
	// CGI
	// GET
}

bool	Get::_check(void) const
{
	Response	rep;
	File const	file(m_path);

	if (m_header.find("Content-Length") != m_header.end() || m_header.find("Transfer-encoding") != m_header.end())
	{
		rep = Response::create_error("413", m_location);
		_send(rep);
		return (false);
	}
	if (!file.exist())
	{
		rep = Response::create_error("404", m_location);
		_send(rep);
		return (false);
	}
	else if (!file.is_readable())
	{
		rep = Response::create_error("403", m_location);
		_send(rep);
		return (false);
	}
	else if (m_header.find("Range") != m_header.end())
	{
		rep = Response::create_error("416", m_location);
		_send(rep);
		return (false);
	}
	return (true);	
}

bool	Get::_manageDir(void)
{
	Response	rep;

	if (*(m_path.end() - 1) != '/')
		rep = Response::create_redirect("302", m_header.at("uri") + "/");
	else if (m_location->getAutoindex())
		rep = Response::create_index(m_path, m_location, m_header.at("uri"));
	else
	{
		rep = Response::create_error("403", m_location);
		_send(rep);
		return (false);
	}
	if (!_send(rep))
		return (false);
	return (true);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */