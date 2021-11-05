#include "Post.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Post::Post()
: Request() 
{
}

Post::Post(Post const & src)
: Request(src)
{
}

Post::Post(Request const & src)
: Request(src)
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

bool	Post::execute(ASocket ** ptr)
{
	if (ptr)
		*ptr = this;
	// check_post (peut-etre pas ici)
	// RECV -> buff
	// Unchunk ?
	// Upload ? -> convert to upload & exec
	// CGI ? wait for full body -- convert to CGI -- exec
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */