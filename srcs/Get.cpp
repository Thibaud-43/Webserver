#include "Get.hpp"
#include "CgiGet.hpp"
#include "ChunkGet.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Get::Get()
: Request(), m_cgi_pass(_cgiPass())
{
}

Get::Get( const Get & src )
: Request(src), m_cgi_pass(src.m_cgi_pass)
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
		return (m_fd.epollCtlSwitch_w());
	if (m_path.is_directory())
		return (_manageDir());
	else if (m_cgi_pass)
		return (_start_cgi(ptr));
	else
		return (_get(ptr));
}

bool	Get::_check(void)
{
	if (m_header.find("Content-Length") != m_header.end() || m_header.find("Transfer-encoding") != m_header.end())
		m_rep = Response::create_error("413", m_location);
	else if (!m_path.exist())
		m_rep = Response::create_error("404", m_location);
	else if (!m_path.is_readable())
		m_rep = Response::create_error("403", m_location);
	else if (m_header.find("Range") != m_header.end())
		m_rep = Response::create_error("416", m_location);
	else if (!m_path.is_directory() && !m_path.is_regular())
		m_rep = Response::create_error("415", m_location);
	else
		return (true);	
	return (false);
}

bool	Get::_manageDir(void)
{
	if (*(m_path.getPath().end() - 1) != '/')
		m_rep = Response::create_redirect("302", m_header.at("uri") + "/");
	else if (m_location->getAutoindex())
		m_rep = Response::create_index(m_path.getPath(), m_location, m_header.at("uri"));
	else
	{
		m_rep = Response::create_error("403", m_location);
		return (m_fd.epollCtlSwitch_w());
	}
	if (m_header.find("Connection") != m_header.end() && m_header.at("Connection") == "close")
		m_rep.append_to_header("Connection: close");
	return (m_fd.epollCtlSwitch_w());
}

Location::file_t const *	Get::_cgiPass(void) const
{
	Location::cgi_t	const & cgi = m_location->getCGIPass();

	for (Location::cgi_t::const_iterator it = cgi.begin() ; it != cgi.end(); it++)
	{
		if (it->first.size() > m_path.getPath().size())
			break ;
		else if (m_path.getPath().find(it->first, m_path.getPath().size() - it->first.size()) != std::string::npos)
			return (&(it->second));
	}
	return (NULL);
}

bool	Get::_start_cgi(ASocket ** ptr)
{
	File	f(*m_cgi_pass);
	CgiGet	*cgi;
	
	if (!f.is_executable())
	{
		m_rep = Response::create_error("500", m_location);
		return (m_fd.epollCtlSwitch_w());
	}
	cgi = new CgiGet(*this);
	if (ptr)
		*ptr = cgi;
	ASocket::addSocket(cgi);
	if (!cgi->start())
	{
		m_rep = Response::create_error("500", m_location);
		return (m_fd.epollCtlSwitch_w());
	}
	return ((*ptr)->execute(ptr));
}

bool	Get::_get(ASocket ** ptr)
{
	if (m_path.size() > READ_SIZE)
	{
		ChunkGet	*elem = new ChunkGet(*this);

		if (ptr)
			*ptr = elem;
		ASocket::addSocket(elem);
		return ((*ptr)->execute(ptr));
	}
	m_rep.start_header("200");
	if (m_header.find("Connection") != m_header.end() && m_header.at("Connection") == "close")
		m_rep.append_to_header("Connection: close");
	else
		m_rep.append_to_header("Connection: keep-alive");
	m_rep.append_to_header("Content-length: " + _ltostr(m_path.size()));
	_addFile();
	return (m_fd.epollCtlSwitch_w());
}

bool	Get::_addFile(void)
{
	std::ifstream	fstream(m_path.getPath().data());
	char			buff[READ_SIZE + 1];

	if (!fstream.is_open())
	{
		m_rep = Response::create_error("500", m_location);
		return (false);
	}
	try
	{
		fstream.read(buff, READ_SIZE);
	}
	catch(const std::exception& e)
	{
		m_rep = Response::create_error("500", m_location);
		return (false);
	}
	buff[fstream.gcount()] = 0;
	m_rep.append_to_body(buff);
	return (true);
}

std::string	Get::_ltostr(size_t const & len) const
{
	std::stringstream	ss;

	ss << len;
	return (ss.str());
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */