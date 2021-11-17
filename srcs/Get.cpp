#include "Get.hpp"
#include "CgiGet.hpp"

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
	else if (!m_path.is_directory() && !m_path.is_regular())
		rep = Response::create_error("415", m_location);
	else if (m_path.size() > MAX_SERVER_BODY_SIZE)
		rep = Response::create_error("415", m_location);
	else
		return (true);	
	_send(rep);
	return (false);
}

bool	Get::_manageDir(ASocket ** ptr)
{
	Response	rep;
	bool		ret = true;

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
	if (m_header.find("Connection") != m_header.end() && m_header.at("Connection") == "close")
	{
		ret = false;
		rep.append_to_header("Connection: close");
	}
	if (!_send(rep))
		return (false);
	_convert<Client>(ptr);
	return (ret);
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
		_send(Response::create_error("500", m_location));
		return (false);
	}
	cgi = new CgiGet(*this);
	if (ptr)
		*ptr = cgi;
	ASocket::addSocket(cgi);
	if (!cgi->start())
		return (false);
	return ((*ptr)->execute(ptr));
}

bool	Get::_get(ASocket ** ptr)
{
	Response	rep;
	bool		ret = true;

	rep.start_header("200");
	if (m_header.find("Connection") != m_header.end() && m_header.at("Connection") == "close")
	{
		ret = false;
		rep.append_to_header("Connection: close");
	}
	else
		rep.append_to_header("Connection: keep-alive");
	if (m_path.size() > READ_SIZE)
	{
		rep.append_to_header("Transfer-Encoding: chunked");
		_send(rep);
		if (!_sendChunkedFile())
			return (false);
	}
	else 
	{
		rep.append_to_header("Content-length: " + _ltostr(m_path.size()));
		_send(rep);
		if (!_sendFile())
			return (false);
	}
	if (ret)
		_convert<Client>(ptr);
	return (ret);
}

bool	Get::_sendChunkedFile(void) const
{
	std::ifstream	fstream(m_path.getPath().data(), std::ios_base::in | std::ios_base::binary);
	char			buff[READ_SIZE + 1];
	std::string		body;

	if (!fstream.is_open())
	{
		_send(Response::create_error("500", m_location));
		return (false);
	}
	try
	{
		fstream.read(buff, READ_SIZE);
		while (fstream.gcount())
		{
			if (!_chunk_size_to_client(fstream.gcount()))
				return (false);
			buff[fstream.gcount()] = 0;
			body = buff;
			body += "\r\n";
			if (!_send(body))
				return (false);
			fstream.read(buff, READ_SIZE);
		}
		if (!_send("0\r\n\r\n"))
			return (false);
	}
	catch(const std::exception& e)
	{
		return (false);
	}
	return (true);
}

bool	Get::_sendFile(void) const
{
	std::ifstream	fstream(m_path.getPath().data());
	char			buff[READ_SIZE + 1];

	if (!fstream.is_open())
	{
		_send(Response::create_error("500", m_location));
		return (false);
	}
	try
	{
		fstream.read(buff, READ_SIZE);
	}
	catch(const std::exception& e)
	{
		return (false);
	}
	buff[fstream.gcount()] = 0;
	return (_send(buff));
}

std::string	Get::_ltostr(size_t const & len) const
{
	std::stringstream	ss;

	ss << len;
	return (ss.str());
}

bool	Get::_chunk_size_to_client(std::streamsize const & size) const
{
	std::stringstream stream;

	stream << std::hex << size << "\r\n";
	return (_send(stream.str()));
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */