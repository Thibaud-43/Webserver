#include "Client.hpp"
#include "Request.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Client::Client()
: ASocket(), m_buff(std::string())
{
	memset(&m_remote_addr, 0, sizeof(m_remote_addr));
}

Client::Client( const Client & src )
: ASocket(src), m_buff(src.m_buff)
{
	m_remote_addr.sin_addr.s_addr = src.m_remote_addr.sin_addr.s_addr;
	m_remote_addr.sin_family = src.m_remote_addr.sin_family;
	m_remote_addr.sin_port = src.m_remote_addr.sin_port;
	memset(&m_remote_addr.sin_zero, 0, sizeof(m_remote_addr.sin_zero));
}

Client::Client( const int & fd, Server const * server, struct sockaddr_in const & remote_addr)
: ASocket(fd, server), m_buff(std::string())
{
	m_fd.epollCtlAdd();
	m_fd.makeFdNonBlocking();
	m_remote_addr.sin_addr.s_addr = remote_addr.sin_addr.s_addr;
	m_remote_addr.sin_family = remote_addr.sin_family;
	m_remote_addr.sin_port = remote_addr.sin_port;
	memset(&m_remote_addr.sin_zero, 0, sizeof(m_remote_addr.sin_zero));
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

	for (;;)
	{
		bytes_read = recv(getFd(), read_buffer, READ_SIZE, 0);
		if (bytes_read < 0)
		{
			close(getFd());
			return false;
		}
		else if (bytes_read == 0)
		{
			return true;
		}
		else if (bytes_read == READ_SIZE)
		{
			read_buffer[bytes_read] = 0;
			m_buff += read_buffer;
			if (m_buff.size() >= HEADER_SIZE_LIMIT && m_buff.find("\r\n\r\n") == std::string::npos)
			{
				_send(Response::create_error("431", NULL));
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

bool	Client::execute(ASocket ** ptr)
{
	Request		*request;
	if (ptr)
		*ptr = this;
	request = new Request(*this);
	if (ptr)
		*ptr = request;
	ASocket::addSocket(request);
	return request->execute(ptr);
}

bool	Client::_send(Response const & rep) const
{
	std::string	content = rep.getContent();

	if (send(getFd(), content.c_str(), content.size(), 0) == -1)
		return (false);
	return (true);
}

bool	Client::_send(std::string const & rep) const
{
	if (send(getFd(), rep.c_str(), rep.size(), 0) == -1)
		return (false);
	return (true);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */