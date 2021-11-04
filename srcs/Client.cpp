#include "Client.hpp"
#include "Request.hpp"
/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Client::Client(): ASocket(), m_buff("")
{
}

Client::Client( const Client & src ): ASocket(src), m_buff(src.m_buff)
{
}

Client::Client( const int & fd): ASocket(fd), m_buff("")
{

}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Client::~Client()
{
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

/*
** --------------------------------- METHODS ----------------------------------
*/

bool	Client::alive(void) const
{
	return (time(NULL) - m_clock < LIFETIME);
}

bool	Client::_fillBuffer(void)
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
		}
		else
		{ 
			read_buffer[bytes_read] = 0;
			m_buff += read_buffer;
			return true;
		}
	}
}

bool	Client::execute(ASocket * ptr)
{
	Request		*request;
	if (!_fillBuffer())
		return false;
	if (m_buff.empty())
		return true;
	request = new Request(*this);
	ptr = request;
	ASocket::addSocket(request);
	return request->execute(ptr);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */