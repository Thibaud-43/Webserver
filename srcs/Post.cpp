#include "Post.hpp"
#include "CgiPost.hpp"
#include "Upload.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Post::Post()
: Request(), m_cgi_pass(_cgiPass()), m_body(std::string())
{
}

Post::Post(Post const & src)
: Request(src), m_cgi_pass(src.m_cgi_pass), m_body(src.m_body)
{
}

Post::Post(Request const & src)
: Request(src), m_cgi_pass(_cgiPass()), m_body(std::string())
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Post::~Post()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

/*
** --------------------------------- METHODS ----------------------------------
*/
bool	Post::_fillBuffer(void)
{
	size_t              bytes_read = 0;
	char                read_buffer[READ_SIZE_RECV + 1];


	bytes_read = recv(getFd(), read_buffer, READ_SIZE_RECV, 0);
	if (bytes_read < 0)
	{
		close(getFd());
		return false;
	}
	else if (bytes_read == 0)
	{
		return true;
	}
	else if (bytes_read == READ_SIZE_RECV)
	{
		read_buffer[bytes_read] = 0;
		m_buff += read_buffer;
		if (m_buff.size() > m_location->getBodySize())
		{
			m_rep = Response::create_error("413", NULL);
			return false;
		}
	}
	else
	{
		read_buffer[bytes_read] = 0;
		m_buff += read_buffer;
		if (m_buff.size() > m_location->getBodySize())
		{
			m_rep = Response::create_error("413", &m_server->getParams());
			return (false);		
		}
		return true;
	}
	return true;

}

bool	Post::_check(void)
{
	if (m_header.find("Content-Length") == m_header.end() && m_header.find("Transfer-Encoding") == m_header.end())
		m_rep = Response::create_error("411", m_location);
	else if (m_header.find("Content-Length") != m_header.end() && _strToSize(m_header.at("Content-Length")) > m_location->getBodySize())
		m_rep = Response::create_error("413", m_location);
	else
		return (true);
	return (false);
}

bool	Post::_start_cgi(ASocket ** ptr)
{
	File	f(*m_cgi_pass);
	CgiPost	*cgi;
	
	if (!f.is_executable())
	{
		m_rep = Response::create_error("500", m_location);
		return (false);
	}
	cgi = new CgiPost(*this);
	if (ptr)
		*ptr = cgi;
	ASocket::addSocket(cgi);
	return (cgi->entry(ptr));
}

bool	Post::_upload(ASocket ** ptr)
{
	Upload	*upload = new Upload(*this);

	if (ptr)
		*ptr = upload;
	ASocket::addSocket(upload);
	return (upload->entry(ptr));
}

bool	Post::execute(ASocket ** ptr)
{
	if (ptr)
		*ptr = this;
	if (!_check())
		return (m_fd.epollCtlSwitch_w());
	if (m_cgi_pass)
		return (_start_cgi(ptr));
	else if (m_location->getUpload())
		return (_upload(ptr));
	else 
	{
		m_rep = Response::create_error("403", m_location);
		return (m_fd.epollCtlSwitch_w());
	}
}

Location::file_t const *	Post::_cgiPass(void) const
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

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */