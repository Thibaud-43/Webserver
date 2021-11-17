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
	if (!_check())
		return (m_fd.epollCtlSwitch_w());
	_delete();
	return (m_fd.epollCtlSwitch_w());
}

bool	Delete::_check(void)
{
	if (m_header.find("Content-Length") != m_header.end() || m_header.find("Transfer-encoding") != m_header.end())
		m_rep = Response::create_error("413", m_location);
	else if (!m_path.exist())
		m_rep = Response::create_error("404", m_location);
	else if (m_path.is_directory() || !m_path.is_writable())
		m_rep = Response::create_error("403", m_location);
	else
		return (true);
	return (false);
}

bool	Delete::_delete(void)
{
	if (remove(m_path.getPath().c_str()))
	{
		m_rep = Response::create_error("500", m_location);
		return (false);
	}
	m_rep.start_header("200");
	m_rep.append_to_body("<html>\n<body>\n<h1>File deleted.</h1>\n</body>\n<html>\n");
	m_rep.add_content_length();
	if (m_header.find("Connection") != m_header.end() && m_header.at("Connection") == "close")
	{
		m_rep.append_to_header("Connection: close");
		return (false);
	}
	m_rep.append_to_header("Connection: keep-alive");
	return (true);
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */