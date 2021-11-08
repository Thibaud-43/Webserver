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
: Request(src), m_cgi_pass(_cgiPass()), m_body(src.m_body)
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
	size_t              bytes_read;
	char                read_buffer[READ_SIZE + 1];
	size_t              read_buffer_size = sizeof(read_buffer);

	for (;;)
	{
		memset(read_buffer, 0, read_buffer_size);
		bytes_read = recv(getFd(), read_buffer, read_buffer_size, 0);
		if (bytes_read < 0)
		{
			close(getFd());
			return false;
		}
		else if (bytes_read == 0)
		{
			return true;
		}
		else if (bytes_read == read_buffer_size)
		{
			read_buffer[bytes_read] = 0;
			m_buff += read_buffer;
			if (!_decrement(m_buff.size()))
			{
				_send(Response::create_error("413", &m_server->getParams()));
				return (false);		
			}
			else if (m_buff.size() >= m_location->getBodySize())
			{
				_send(Response::create_error("413", NULL));
				return false;
			}
		}
		else
		{
			if (!_decrement(m_buff.size()))
			{
				_send(Response::create_error("413", &m_server->getParams()));
				return (false);		
			}
			read_buffer[bytes_read] = 0;
			m_buff += read_buffer;
			return true;
		}
	}
}

bool	Post::_check(void) const
{
	Response	rep;

	if (m_header.find("Content-Length") == m_header.end() || m_header.find("transfer-Encoding") == m_header.end())
		rep = Response::create_error("411", m_location);
	else
		return (true);
	_send(rep);
	return (false);
}

bool	Post::execute(ASocket ** ptr)
{
	if (ptr)
		*ptr = this;
	if (!_check())
		return (false);
	if (m_cgi_pass)
	{
		_convert<CgiPost>(ptr);
		return ((*ptr)->execute(ptr));
	}
	else if (m_location->getUpload())
	{
		_convert<Upload>(ptr);
		return ((*ptr)->execute(ptr));
	}
	else 
	{
		_send(Response::create_error("403", m_location));
		return (false);
	}
	return (true);
}

Location::file_t const *	Post::_cgiPass(void) const
{
	Location::cgi_t	const & cgi = m_location->getCGIPass();

	for (Location::cgi_t::const_iterator it = cgi.begin() ; it != cgi.end(); it++)
	{
		if (it->first.size() > static_cast<size_t>(m_path.size()))
			break ;
		else if (m_path.getPath().find(it->first, m_path.size() - it->first.size()) != std::string::npos)
			return (&(it->second));
	}
	return (NULL);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */