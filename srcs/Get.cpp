#include "Get.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Get::Get()
: Request(), m_cgi_pass(_cgiPass())
{
}

Get::Get( const Get & src )
: Request(src), m_cgi_pass(_cgiPass())
{
}

Get::Get(Request const & src)
: Request(src), m_cgi_pass(_cgiPass())
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
	if (ptr)
		*ptr = this;
	if (!_check())
		return (false);
	if (m_path.is_directory())
		return (_manageDir(ptr));
	else if (m_cgi_pass)
		return (_start_cgi(ptr));
	else
		return (_get(ptr));
}

bool	Get::_check(void) const
{
	Response	rep;

	if (m_header.find("Content-Length") != m_header.end() || m_header.find("Transfer-encoding") != m_header.end())
		rep = Response::create_error("413", m_location);
	else if (!m_path.exist())
		rep = Response::create_error("404", m_location);
	else if (!m_path.is_readable())
		rep = Response::create_error("403", m_location);
	else if (m_header.find("Range") != m_header.end())
		rep = Response::create_error("416", m_location);
	else
		return (true);	
	_send(rep);
	return (false);
}

bool	Get::_manageDir(ASocket ** ptr)
{
	Response	rep;

	if (*(m_path.getPath().end() - 1) != '/')
		rep = Response::create_redirect("302", m_header.at("uri") + "/");
	else if (m_location->getAutoindex())
		rep = Response::create_index(m_path.getPath(), m_location, m_header.at("uri"));
	else
	{
		rep = Response::create_error("403", m_location);
		_send(rep);
		return (false);
	}
	if (!_send(rep))
		return (false);
	_convert<Client>(ptr);
	return (true);
}

Location::file_t const *	Get::_cgiPass(void) const
{
	Location::cgi_t	const & cgi = m_location->getCGIPass();

	for (Location::cgi_t::const_iterator it = cgi.begin() ; it != cgi.end(); it++)
	{
		if (it->first.size() > m_path.size())
			break ;
		else if (m_path.getPath().find(it->first, m_path.size() - it->first.size()) != std::string::npos)
			return (&(it->second));
	}
	return (NULL);
}

bool	Get::_start_cgi(ASocket ** ptr)
{

}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */