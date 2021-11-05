#include "Post.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Post::Post()
: Request(), m_cgi_pass(_cgiPass()) 
{
}

Post::Post(Post const & src)
: Request(src), m_cgi_pass(_cgiPass())
{
}

Post::Post(Request const & src)
: Request(src), m_cgi_pass(_cgiPass())
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
			if (m_buff.size() >= m_location->getBodySize())
			{
				_send(Response::create_error("413", NULL));
				return false;
			}
		}
		else
		{ 
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
		// check -- convert to CGI -- exec
	}
	else if (m_location->getUpload())
	{
		// check -- convert to upload & exec
	}
	// ?
}

Location::file_t const *	Post::_cgiPass(void) const
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

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */