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

bool	Delete::execute(ASocket ** ptr)
{
	if (ptr)
		*ptr = this;
	if (!_check() || !_delete())
		return (false);
	_convertToClient(ptr);
	return (true);
}

bool	Delete::_check(void) const
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
	else if (!file.is_writable())
	{
		rep = Response::create_error("403", m_location);
		_send(rep);
		return (false);
	}
	return (true);
}

bool	Delete::_delete(void) const
{
	Response	rep;

	if (remove(m_path.c_str()))
	{
		rep = Response::create_error("500", m_location);
		_send(rep);
		return (false);
	}
	rep.start_header("200");
	rep.append_to_body("<html>\n<body>\n<h1>File deleted.</h1>\n</body>\n<html>\n");
	rep.add_content_length();
	if (m_header.find("Connection") != m_header.end() && m_header.at("Connection") == "close")
	{
		rep.append_to_header("Connection: close");
		_send(rep);
		return (false);
	}
	rep.append_to_header("Connection: keep-alive");
	if (!_send(rep))
		return (false);
	return (true);
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */